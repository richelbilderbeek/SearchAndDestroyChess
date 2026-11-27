#include "fileio.h"





#include <cassert>
#include <fstream>

#include <boost/xpressive/xpressive.hpp>

#include <QDir>
#include <QFile>

#include "filecopymode.h"



ribi::fileio::FileIo::FileIo()
{

}

std::string ribi::fileio::FileIo::ConvertPathToUnix(const std::string& path) const noexcept
{
  std::string s = path;
  std::replace(std::begin(s),std::end(s),'\\','/');
  assert(IsUnixPath(s));
  return s;
}

void ribi::fileio::FileIo::CopyFile(
  const std::string& fileNameFrom,
  const std::string& fileNameTo,
  const CopyMode copy_mode
) const
{
  assert(IsRegularFile(fileNameFrom) && "Cannot copy a non-existing file");
  if (copy_mode == CopyMode::prevent_overwrite && IsRegularFile(fileNameTo))
  {
    throw std::logic_error("Copying to an existing file is not allowed");
  }
  if (IsRegularFile(fileNameTo))
  {
    //DeleteFile ensures a correct deletion
    DeleteFile(fileNameTo);
  }
  std::ifstream in{fileNameFrom.c_str()};
  std::ofstream out{fileNameTo.c_str()};
  out << in.rdbuf();
  out.close();
  in.close();
}

void ribi::fileio::FileIo::CreateFolder(const std::string& folder) const
{
  if(IsFolder(folder))
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "can only create folders that do not exist yet, "
      << "folder name supplied: '"
      << folder << "' was found to exist"
    ;
    throw std::invalid_argument(msg.str());
  }
  const auto cmd = "mkdir " + folder;
  const auto error = std::system(cmd.c_str());
  assert(!error && "Assume mkdir works under both Windows and Linux");
  if (error)
  {
    std::stringstream s;
    s << "ribi::fileio::FileIo::CreateFolder: "
      << "CreateFolder failed in creating the folder '" << folder << "': "
    ;
    throw std::runtime_error(s.str());
  }
  assert(IsFolder(folder) && "it should work");
}

void ribi::delete_file(const std::string& filename)
{
  if(!is_regular_file(filename))
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "can only delete existing files, "
      << "filename supplied: '"
      << filename << "' was not found"
    ;
    throw std::invalid_argument(msg.str());
  }
  std::remove(filename.c_str());

  //Under Windows, readonly files must be made deleteable
  if (is_regular_file(filename))
  {
    #ifdef _WIN32
    const auto cmd = "attrib -r " + filename;
    std::system(cmd.c_str());
    std::remove(filename.c_str());
    #endif
  }

  if(is_regular_file(filename))
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "failed to delete existing file '"
      << filename << "'"
    ;
    throw std::invalid_argument(msg.str());
  }
}

void ribi::fileio::FileIo::DeleteFile(const std::string& filename) const
{
  delete_file(filename);
}

void ribi::fileio::FileIo::DeleteFolder(const std::string& folder) const
{
  if (!IsFolder(folder))
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "can only delte folders that exist, "
      << "folder name supplied: '"
      << folder << "' was found not to exist"
    ;
    throw std::invalid_argument(msg.str());
  }

  //Delete all files
  for (const auto& subfolder: GetFoldersInFolder(folder))
  {
    DeleteFolder(
      (folder.empty() ? folder : folder + GetPathSeperator())
      + subfolder
    );
  }
  assert(GetFoldersInFolder(folder).empty());
  for (const auto& filename: GetFilesInFolder(folder))
  {
    DeleteFile(
      (folder.empty() ? folder : folder + GetPathSeperator())
      + filename
   );
  }
  assert(GetFilesInFolder(folder).empty());
  const auto cmd = "rmdir " + folder;
  const auto error = std::system(cmd.c_str());
  assert(!error && "Assume rmdir works under both Windows and Linux");
  if (error)
  {
    std::stringstream s;
    s << "ribi::fileio::FileIo::DeleteFolder: "
      << "DeleteFolder failed in deleting the folder '" << folder << "': "
      << "failed by system call"
    ;
    throw std::runtime_error(s.str());
  }
  if (IsFolder(folder))
  {
    std::stringstream s;
    s << "ribi::fileio::FileIo::DeleteFolder: "
      << "DeleteFolder failed in deleting the folder '" << folder << "'"
    ;
    throw std::runtime_error(s.str());
  }
}

bool ribi::fileio::FileIo::FilesAreIdentical(
  const std::string& filename_a,
  const std::string& filename_b
) const
{
  if(!IsRegularFile(filename_a))
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "can only compare existing files, "
      << "filename_a supplied: '"
      << filename_a << "' was not found"
    ;
    throw std::invalid_argument(msg.str());
  }
  if(!IsRegularFile(filename_b))
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "can only compare existing files, "
      << "filename_a supplied: '"
      << filename_b << "' was not found"
    ;
    throw std::invalid_argument(msg.str());
  }
  assert(IsRegularFile(filename_a) && "File must exist to be compared");
  assert(IsRegularFile(filename_b) && "File must exist to be compared");
  const auto v = FileToVector(filename_a);
  const auto w = FileToVector(filename_b);
  return v == w;
}

std::string ribi::fileio::FileIo::FileToStr(
  const std::string& filename) const
{
  if(!IsRegularFile(filename))
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "can only convert existing files, "
      << "filename supplied: '"
      << filename << "' was not found"
    ;
    throw std::invalid_argument(msg.str());
  }

  std::string s;
  for (const auto& t: FileToVector(filename))
  {
    s += t + '\n';
  }
  if (!s.empty()) s.pop_back();
  return s;
}

std::vector<std::string> ribi::fileio::FileIo::FileToVector(
  const std::string& filename) const
{
  if(!IsRegularFile(filename))
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "can only convert existing files, "
      << "filename supplied: '"
      << filename << "' was not found"
    ;
    throw std::invalid_argument(msg.str());
  }
  assert(IsRegularFile(filename));
  assert(!IsFolder(filename));
  std::vector<std::string> v;
  std::ifstream in{filename.c_str()};
  assert(in.is_open());
  //Without this test in release mode,
  //the program might run indefinitely when the file does not exists
  if (!in.is_open())
  {
    const std::string s{"ERROR: file does not exist: " + filename};
    throw std::logic_error{s.c_str()};
  }
  for (int i=0; !in.eof(); ++i)
  {
    std::string s;
    std::getline(in,s);
    v.push_back(s); //Might throw std::bad_alloc
  }
  //Remove empty line at back of vector
  if (!v.empty() && v.back().empty()) v.pop_back();
  return v;
}

std::string ribi::fileio::FileIo::GetExtension(
  const std::string& filename) const noexcept
{
  return GetExtensionWithDot(filename);
}

std::string ribi::fileio::FileIo::GetExtensionNoDot(const std::string& filename) const noexcept
{
  static const boost::xpressive::sregex rex{
    boost::xpressive::sregex::compile("(.*)?(\\.)([A-Za-z]*)?" )
  };
  boost::xpressive::smatch what;

  if( boost::xpressive::regex_match( filename, what, rex ) )
  {
    return what[3];
  }

  return "";
}

std::string ribi::fileio::FileIo::GetExtensionWithDot(const std::string& filename) const noexcept
{
  return
    ( std::count(
        std::begin(filename),
        std::end(filename),
        '.'
    )
    ? "."
    : ""
    )
    + GetExtensionNoDot(filename);
}

std::string ribi::fileio::FileIo::GetFileBasename(const std::string& filename) const
{
  const boost::xpressive::sregex rex{
    boost::xpressive::sregex::compile("((.*)(/|\\\\))?([A-Za-z0-9_-]*)((\\.)([A-Za-z0-9]*))?" )
  };
  boost::xpressive::smatch what;

  if(boost::xpressive::regex_match( filename, what, rex ))
  {
    return what[4];
  }

  return "";
}

std::vector<std::string> ribi::fileio::FileIo::GetFilesInFolder(
  const std::string& folder) const
{
  assert(IsFolder(folder));
  QDir dir{folder.c_str()};
  dir.setFilter(QDir::Files);
  const QFileInfoList list{dir.entryInfoList()};

  //Convert QFileInfoList to std::vector<std::string> of filenames
  std::vector<std::string> v;
  const int size{static_cast<int>(list.size())};
  for (int i{0}; i != size; ++i)
  {
    assert(i >= 0);
    assert(i < static_cast<int>(list.size()));
    const std::string file_name{list[i].fileName().toStdString()};
    v.push_back(file_name);
  }
  return v;
}

std::vector<std::string> ribi::fileio::FileIo::GetFilesInFolderRecursive(
  const std::string& root_folder) const
{
  assert(IsFolder(root_folder));

  //Files in root_folder
  std::vector<std::string> v;
  {
    const std::vector<std::string> files_here{
      GetFilesInFolder(root_folder)
    };
    //Copy the files and folders with path added
    std::transform(
      std::begin(files_here),
      std::end(files_here),
      std::back_inserter(v),
      [this,root_folder](const std::string& filename)
      {
        const std::string filename_here{
          root_folder + GetPathSeperator() + filename
        };
        assert(IsRegularFile(filename_here));
        return filename_here;
      }
    );
  }
  //Folders in root_folder
  std::vector<std::string> folders_todo;
  {
    const auto folders_here = GetFoldersInFolder(root_folder);
    std::transform(
      std::begin(folders_here),
      std::end(folders_here),
      std::back_inserter(folders_todo),
      [this,root_folder](const std::string& foldername)
      {
        const auto folder_here = root_folder + GetPathSeperator() + foldername;
        assert(IsFolder(folder_here));
        return folder_here;
      }
    );
  }

  //Search through all sub folders
  while (!folders_todo.empty())
  {
    const auto folder_todo =
      folders_todo.back(); //Read from the back, so push_back can be used

    //Done with this folder
    folders_todo.pop_back();

    assert( (folders_todo.empty() || folders_todo.back() != folder_todo)
      && "Next folder must not be the one that is just processed");


    const auto files_here = GetFilesInFolder(folder_todo);
    const auto folders_here = GetFoldersInFolder(folder_todo);

    //Copy the files and folders with path added
    std::transform(
      std::begin(files_here),
      std::end(files_here),
      std::back_inserter(v),
      [this,folder_todo](const std::string& filename)
      {
        const std::string file_here {
          folder_todo + GetPathSeperator() + filename
        };
        assert(IsRegularFile(file_here));
        return file_here;
      }
    );
    std::transform(
      std::begin(folders_here),
      std::end(folders_here),
      std::back_inserter(folders_todo),
      [this,folder_todo](const std::string& foldername)
      {
        assert(!foldername.empty());
        const std::string subfolder_name {
          folder_todo + GetPathSeperator() + foldername
        };
        assert(subfolder_name != folder_todo);
        return subfolder_name;
      }
    );
  }
  return v;
}

std::vector<std::string> ribi::fileio::FileIo::GetFilesInFolderByRegex(
  const std::string& folder,
  const std::string& regex_str
) const
{
  //Get all filenames
  assert(IsFolder(folder));
  const auto v = GetFilesInFolder(folder);

  //Create the regex
  const auto rex = boost::xpressive::sregex::compile(regex_str);

  //Create the resulting std::vector
  std::vector<std::string> w;

  //Copy all filenames matching the regex in the resulting std::vector
  std::copy_if(std::begin(v),std::end(v),std::back_inserter(w),
    [rex](const std::string& s)
    {
      boost::xpressive::smatch what;
      return boost::xpressive::regex_match(s, what, rex);
    }
  );

  return w;
}

std::vector<std::string> ribi::fileio::FileIo::GetFoldersInFolder(
  const std::string& folder) const
{
  assert(IsFolder(folder));
  QDir dir{folder.c_str()};
  dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot );
  //const QFileInfoList list{dir.entryInfoList()};
  const auto list = dir.entryInfoList();

  //Convert QFileInfoList to std::vector<std::string> of filenames
  std::vector<std::string> v;
  const auto size = list.size();
  for (auto i = 0; i != size; ++i)
  {
    const auto folder_name = list.at(i).fileName().toStdString();
    assert(!folder_name.empty());
    v.push_back(folder_name);
  }
  #ifndef NDEBUG
  for (const auto& s: v)
  {
    assert(
      std::count(std::begin(v),std::end(v),s) == 1
      && "Every folder name is unique");
  }
  #endif
  return v;
}

std::string ribi::fileio::FileIo::GetPath(const std::string& filename) const
{
  const auto a = filename.rfind("\\",filename.size());
  const auto b = filename.rfind("/",filename.size());
  std::size_t i{0};
  if (a != std::string::npos) { i = a; }
  if (b != std::string::npos) { i = std::max(i,b); }
  //assert(i >= 0);
  assert(i < filename.size());
  return filename.substr(0,i);
}

std::string ribi::fileio::FileIo::GetPathSeperator() const noexcept
{
  #ifdef _WIN32
  return "\\";
  #else
  return "/";
  #endif
}

std::string ribi::fileio::FileIo::GetSuperFolder(const std::string& folder) const
{
  const auto a = folder.rfind("\\",folder.size());
  const auto b = folder.rfind("/",folder.size());
  std::size_t i{0};
  if (a != std::string::npos) { i = a; }
  if (b != std::string::npos) { i = std::max(i,b); }

  //assert(i >= 0);
  assert(i < folder.size());
  return
    i == 0
    ? ""
    : folder.substr(0,i);
}

std::string ribi::get_temp_filename(const std::string& post)
{
  //Limit the number of searches, to prevent the program from freezing
  //It might occur that the first random names are taken, because
  //of working without a randomize timer first
  for (auto i = 0; i!=1000; ++i)
  {
    std::stringstream s;
    s << "tmpfilename"
      << std::rand()
      << std::rand()
      << std::rand()
      << std::rand()
      << post
    ;
    if (!is_regular_file(s.str())) return s.str();
  }

  std::stringstream s;
  s << "ribi::fileio::FileIo::GetTempFileName: "
    << "Could not find a temporary file name"
  ;
  throw std::runtime_error(s.str());
}

std::string ribi::fileio::FileIo::GetTempFileName(const std::string& post) const
{
  return ribi::get_temp_filename(post);
}

std::string ribi::get_temp_filename_simple(const std::string& post)
{
  //Limit the number of searches, to prevent the program from freezing
  for (auto i = 0; i!=1000; ++i)
  {
    std::stringstream s;
    s << "tmp"
      << i
      << post;
    if (!is_regular_file(s.str())) return s.str();
  }

  std::stringstream s;
  s << "ribi::fileio::FileIo::GetTempFileNameSimple: "
    << "Could not find a temporary file name"
  ;
  throw std::runtime_error(s.str());
}

std::string ribi::fileio::FileIo::GetTempFileNameSimple(const std::string& post) const
{
  return ribi::get_temp_filename_simple(post);
}
std::string ribi::fileio::FileIo::GetTempFolderName() const
{
  //Limit the number of searches, to prevent the program from freezing
  //It might occur that the first random names are taken, because
  //of working without a randomize timer first
  for (auto i = 0; i!=1000; ++i)
  {
    std::stringstream s;
    s << "tmpfoldername"
      << std::rand()
      << std::rand()
      << std::rand()
      << std::rand();
    if (!IsFolder(s.str())) return s.str();
  }

  std::stringstream s;
  s << "ribi::fileio::FileIo::GetTempFolderName: "
    << "Could not find a temporary folder name"
  ;
  throw std::runtime_error(s.str());
}

std::string ribi::fileio::FileIo::GetVersion() const noexcept
{
  return "1.4";
}

std::vector<std::string> ribi::fileio::FileIo::GetVersionHistory() const noexcept
{
  return {
    "2013-10-14: Version 1.0: initial version, used free functions only",
    "2013-11-08: Version 1.1: improved FileToVector, improved GetFileBasename, added some functions",
    "2013-12-10: Version 1.2: improved existing function, added some functions",
    "2014-03-24: Version 1.3: put free functions in FileIo class",
    "2014-07-15: Version 1.4: increased use of auto and initializer lists"
  };
}

bool ribi::fileio::FileIo::HasRightPathSeperators(const std::string& path) const noexcept
{
  #ifdef _WIN32
  const auto wrong = '/';
  #else
  const auto wrong = '\\';
  #endif
  return path.find(wrong) == std::string::npos;
}

bool ribi::fileio::FileIo::HasTwoPathSeperators(const std::string& path) const noexcept
{
  return path.find('/') != std::string::npos
    && path.find('\\') != std::string::npos;
}

bool ribi::fileio::FileIo::IsFolder(const std::string& filename) const noexcept
{
  return QDir(filename.c_str()).exists();
}

bool ribi::is_regular_file(const std::string& filename) noexcept
{
  return !QDir(filename.c_str()).exists() && QFile::exists(filename.c_str());
}

bool ribi::fileio::FileIo::IsRegularFile(const std::string& filename) const noexcept
{
  return ::ribi::is_regular_file(filename);
}

bool ribi::fileio::FileIo::IsUnixPath(const std::string& path) const noexcept
{
  if (path.find('\\') != std::string::npos)
  {
    return false;
  }
  return true;
}

std::string ribi::fileio::FileIo::RemovePath(const std::string& filename) const
{
  std::vector<std::size_t> v;
  const auto a = filename.rfind("\\",filename.size());
  if (a != std::string::npos) v.push_back(a);
  const auto b = filename.rfind("/",filename.size());
  if (b != std::string::npos) v.push_back(b);
  if (v.empty()) return filename;
  const auto i = *std::max_element(std::begin(v),std::end(v));
  assert(i < filename.size());
  const auto j = i + 1;
  assert(j < filename.size());
  const auto s = filename.substr(j,filename.size() - j);
  return s;
}

void ribi::fileio::FileIo::RenameFile(
  const std::string& from,
  const std::string& to,
  const RenameMode rename_mode) const
{
  if(!IsRegularFile(from))
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "can only rename existing files, "
      << "filename supplied: '"
      << from << "' was not found"
    ;
    throw std::invalid_argument(msg.str());
  }
  if(from == to)
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "can only rename a file to its same name. "
      << "Filename supplied: '"
      << from << "'"
    ;
    throw std::invalid_argument(msg.str());
  }
  if (rename_mode == RenameMode::prevent_overwrite && IsRegularFile(to))
  {
    throw std::logic_error("Renaming to an existing file is not allowed");
  }

  if (IsRegularFile(to))
  {
    //DeleteFile ensures a correct deletion
    DeleteFile(to);
  }
  const int result{std::rename(from.c_str(),to.c_str())};
  if (result == 0)
  {
    assert( IsRegularFile(to)  );
    assert(!IsRegularFile(from));
  }
  else
  {
    //Delete 'from' manually
    assert( IsRegularFile(to)  );
    DeleteFile(from);
    assert( IsRegularFile(to));
    assert(!IsRegularFile(from));
  }
}

std::string ribi::fileio::FileIo::SimplifyPath(const std::string& s) const noexcept
{
  std::string t{s};
  //UNIX
  while (1)
  {
    static const boost::xpressive::sregex regex{boost::xpressive::sregex::compile(R"((/\w*/\.\./))")};

    const std::string new_t{boost::xpressive::regex_replace(
      t,regex,"/")
    };
    if (t == new_t) break;
    t = new_t;
  }
  //Win32
  while (1)
  {
    static const boost::xpressive::sregex regex{boost::xpressive::sregex::compile(R"((\\\w*\\\.\.\\))")};

    const std::string new_t{
      boost::xpressive::regex_replace(t,regex,"\\")
    };
    if (t == new_t) break;
    t = new_t;
  }
  //UNIX
  while (1)
  {
    static const boost::xpressive::sregex regex{boost::xpressive::sregex::compile(R"((^\w*/\.\.))")};

    const std::string new_t{
      boost::xpressive::regex_replace(t,regex,"")};
    if (t == new_t) break;
    t = new_t;
  }
  //Win32
  while (1)
  {
    static const boost::xpressive::sregex regex = boost::xpressive::sregex::compile(R"((^\w*\\\.\.))");

    const std::string new_t = boost::xpressive::regex_replace(
      t,
      regex,
      ""
    );
    if (t == new_t) break;
    t = new_t;
  }
  return t;
}

void ribi::fileio::FileIo::VectorToFile(
  const std::vector<std::string>& v,
  const std::string& filename,
  const CopyMode copy_mode
) const
{
  if (copy_mode == CopyMode::prevent_overwrite)
  {
    assert(!IsRegularFile(filename) && "File must not exist");
    if (IsRegularFile(filename))
    {
      std::stringstream s;
      s
        << "VectorToFile: not allowed to overwrite file '"
        << filename
        << "'";
      throw std::runtime_error(s.str());
    }
  }
  {
    std::ofstream f(filename.c_str());
    std::copy(
      std::begin(v),
      std::end(v),
      std::ostream_iterator<std::string>(f,"\n")
    );
  }
  assert(IsRegularFile(filename));
}
