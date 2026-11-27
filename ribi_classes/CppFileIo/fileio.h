#ifndef RIBI_FILEIO_H
#define RIBI_FILEIO_H

#include <string>
#include <vector>
#include "fileiofwd.h"
#include "filecopymode.h"


namespace ribi {

///New form: just in namespace ribi

///Delete a file
///Will throw if file is absent or file is removed unsuccessfully
//From http://www.richelbilderbeek.nl/CppDeleteFile.htm
void delete_file(const std::string& filename);

///Obtain the name of a file that does not exist
///Will throw a std::runtime_error in the unlikely
///case it fails after one thousand times
///The 'post' is a string to be at the filename's end
///For example:
/// - GetTempFileName("") -> tmp01234567
/// - GetTempFileName(".txt") -> tmp01234567.txt
std::string get_temp_filename(const std::string& post = "");
std::string get_temp_filename_simple(const std::string& post = "");

///Determines if a filename is a regular file
///From http://www.richelbilderbeek.nl/CppIsRegularFile.htm
bool is_regular_file(const std::string& filename) noexcept;



///Deprecated versions:
namespace fileio {
struct FileIo
{
  enum class RenameMode { allow_overwrite, prevent_overwrite };

  FileIo();

  ///Converts a Windows path to a Unix path. A Unix path will remain unaltered.
  //Does so by replacing '\' by '/'
  std::string ConvertPathToUnix(const std::string& path) const noexcept;

  ///Creates a folder
  ///Will abort if it already exists
  void CreateFolder(const std::string& folder) const;

  ///Copy a file. If prevent_overwrite is set to true, a file
  ///cannot be copied over an existing target. If prevent_overwrite
  ///is set to false, a file can be copied over an existing target,
  ///which will overwrite the target.
  //From http://www.richelbilderbeek.nl/CppCopyFile.htm
  void CopyFile(
    const std::string& fileNameFrom,
    const std::string& fileNameTo,
    const CopyMode copy_mode = CopyMode::prevent_overwrite
  ) const;

  ///Delete a file
  //From http://www.richelbilderbeek.nl/CppDeleteFile.htm
  void DeleteFile(const std::string& filename) const;

  ///Deletes a folder its content recursively
  ///Will throw an exception if this fails
  void DeleteFolder(const std::string& folder) const;

  ///Checks if two files are identical
  ///Will throw std::bad_alloc if at least one of the files is
  ///too big for a std::vector.
  //From http://www.richelbilderbeek.nl/CppFilesAreIdentical.htm
  bool FilesAreIdentical(
    const std::string& filename_a,
    const std::string& filename_b
  ) const;

  ///FileToStr reads a file and converts it to std::string
  ///Throws std::bad_alloc if the file is too big to fit in std::vector
  ///Throws std::logic_error if the file does not exist
  //From http://www.richelbilderbeek.nl/CppFileToVector.htm
  std::string FileToStr(const std::string& filename) const;

  ///FileToVector reads a file and converts it to a std::vector<std::string>
  ///Throws std::bad_alloc if the file is too big to fit in std::vector
  ///Throws std::logic_error if the file does not exist
  //From http://www.richelbilderbeek.nl/CppFileToVector.htm
  std::vector<std::string> FileToVector(const std::string& filename) const;


  ///Returns the extension of a filename, starting with a dot
  //From http://www.richelbilderbeek.nl/CppGetExtension.htm
  std::string GetExtension(const std::string& filename) const noexcept;

  ///Returns the extension of a filename, without a trailing dot
  std::string GetExtensionNoDot(const std::string& filename) const noexcept;

  ///Returns the extension of a filename, with a trailing dot
  std::string GetExtensionWithDot(const std::string& filename) const noexcept;

  ///Obtain the basename of a file or folder
  ///MyFolder/MyFile.txt - > MyFile
  ///MyFolder/MySubFolder -> MySubFolder
  ///GetFileBasename is a replacement of boost::filesystem::basename
  ///From http://www.richelbilderbeek.nl/CppGetFileBasename.htm
  std::string GetFileBasename(const std::string& filename) const;

  ///Obtain all files in a folder.
  ///This does not include those in a subfolder
  //From http://www.richelbilderbeek.nl/CppGetFilesInFolder.htm
  std::vector<std::string> GetFilesInFolder(const std::string& folder) const;

  ///Obtain all files in a folder including thos in a subfolder
  //From http://www.richelbilderbeek.nl/CppGetFilesInFolder.htm
  std::vector<std::string> GetFilesInFolderRecursive(const std::string& folder) const;

  std::vector<std::string> GetFilesInFolderByRegex(
    const std::string& folder,
    const std::string& regex_str
  ) const;

  ///Obtain all folder in a folder.
  ///This does not include those in a subfolder
  //From http://www.richelbilderbeek.nl/CppGetFoldersInFolder.htm
  std::vector<std::string> GetFoldersInFolder(const std::string& folder) const;

  ///Obtain all folder in a folder.
  ///This does include those in a subfolder
  //From http://www.richelbilderbeek.nl/CppGetFoldersInFolder.htm
  std::vector<std::string> GetFoldersInFolderRecursive(const std::string& folder) const;

  ///Returns the path of a filename
  ///From http://www.richelbilderbeek.nl/CppGetPath.htm
  std::string GetPath(const std::string& filename) const;

  ///Returns the path seperator depending on the operating system
  ///For Linux, this is a slash
  ///For Windows, this is a backslash
  ///From http://www.richelbilderbeek.nl/CppGetPathSeperator.htm
  std::string GetPathSeperator() const noexcept;

  ///Returns the superfolder from a path
  ///For example: '/A/B/C' its superfolder is '/A/B'
  std::string GetSuperFolder(const std::string& folder) const;

  ///Obtain the name of a file that does not exist
  ///Will throw a std::runtime_error in the unlikely
  ///case it fails after one thousand times
  ///The 'post' is a string to be at the filename's end
  ///For example:
  /// - GetTempFileName("") -> tmp01234567
  /// - GetTempFileName(".txt") -> tmp01234567.txt
  std::string GetTempFileName(const std::string& post = "") const;
  std::string GetTempFileNameSimple(const std::string& post = "") const;

  ///Obtain the name of a folder that does not exist
  ///Will throw a std::runtime_error in the unlikely
  ///case it fails after one thousand times
  std::string GetTempFolderName() const;

  ///Obtain the version
  std::string GetVersion() const noexcept;

  ///Obtain the version history
  std::vector<std::string> GetVersionHistory() const noexcept;

  ///Checks if the path has only the right path seperators
  ///A Linux path should not contain Windows backslashes
  ///A Windows path should not contain Linux slashes
  bool HasRightPathSeperators(const std::string& path) const noexcept;

  ///Checks if the path has both Linux and Windows path seperators
  bool HasTwoPathSeperators(const std::string& path) const noexcept;

  ///Returns if the name is a folder name
  ///From http://www.richelbilderbeek.nl/CppIsFolder.htm
  bool IsFolder(const std::string& filename) const noexcept;

  ///Determines if a filename is a regular file
  ///From http://www.richelbilderbeek.nl/CppIsRegularFile.htm
  bool IsRegularFile(const std::string& filename) const noexcept;

  ///Checks if the path is a valid UNIX path
  bool IsUnixPath(const std::string& path) const noexcept;

  ///Removes the path of a filename
  ///From http://www.richelbilderbeek.nl/CppRemovePath.htm
  std::string RemovePath(const std::string& filename) const;

  ///Renames a file
  //From http://www.richelbilderbeek.nl/CppRenameFile.htm
  void RenameFile(
    const std::string& from,
    const std::string& to,
    const RenameMode rename_mode = RenameMode::prevent_overwrite
  ) const;


  ///Simplify a path
  ///For example,
  /// /home/richel/Projects/Tools/ToolTestProFile/../../Classes/CppQtAboutDialog/qtaboutdialog.ui
  ///is converted to
  /// /home/richel/Projects/Classes/CppQtAboutDialog/qtaboutdialog.ui
  //From http://www.richelbilderbeek.nl/CppSimplifyPath.htm
  std::string SimplifyPath(const std::string& s) const noexcept;

  ///Convert a std::vector to a file
  void VectorToFile(
    const std::vector<std::string>& v,
    const std::string& filename,
    const CopyMode copy_mode = CopyMode::prevent_overwrite
  ) const;
};

} //~namespace fileio

using FileIo = fileio::FileIo;

} //~namespace ribi

#endif // RIBI_FILEIO_H
