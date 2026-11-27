//---------------------------------------------------------------------------
/*
FileIo, class with file I/O functions
Copyright (C) 2013-2016 Richel Bilderbeek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------
//From http://www.richelbilderbeek.nl/CppFileIo.htm
//---------------------------------------------------------------------------
#include "fileio.h"
#include <fstream>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(ribi_fileio_copyfile_test)
{
  const ribi::FileIo fileio;
  for (int i=0; i!=2; ++i)
  {
    const std::string filename { fileio.GetTempFileName() };
    const std::string filename_copy { fileio.GetTempFileName() };

    //Create a regular file
    BOOST_CHECK(!fileio.IsRegularFile(filename));
    BOOST_CHECK(!fileio.IsRegularFile(filename_copy));
    {
      std::ofstream f;
      f.open(filename.c_str());
      f << "TEMP TEXT";
      f.close();
    }
    BOOST_CHECK(fileio.IsRegularFile(filename));
    BOOST_CHECK(!fileio.IsRegularFile(filename_copy));

    const ribi::fileio::CopyMode copy_mode {
      i == 0
      ? ribi::fileio::CopyMode::allow_overwrite
      : ribi::fileio::CopyMode::prevent_overwrite
    };
    fileio.CopyFile(filename,filename_copy,copy_mode);

    BOOST_CHECK(fileio.IsRegularFile(filename));
    BOOST_CHECK(fileio.IsRegularFile(filename_copy));

    fileio.DeleteFile(filename);
    fileio.DeleteFile(filename_copy);
    BOOST_CHECK(!fileio.IsRegularFile(filename) && "Temporary file must be cleaned up");
    BOOST_CHECK(!fileio.IsRegularFile(filename_copy) && "Temporary file must be cleaned up");
  }
}

BOOST_AUTO_TEST_CASE(ribi_fileio_files_are_identical_test)
{
  const ribi::FileIo fileio;
  const std::vector<std::string> filenames {
    fileio.GetTempFileName(),
    fileio.GetTempFileName(),
    fileio.GetTempFileName()
  };
  const int n_filenames = filenames.size();
  for (const std::string& filename: filenames)
  {
    std::ofstream f(filename.c_str());
    f << filename;
  }

  for (int i=0; i!=n_filenames; ++i)
  {
    const std::string s = filenames[i];
    for (int j=0; j!=n_filenames; ++j)
    {
      const std::string t = filenames[j];
      BOOST_CHECK(fileio.FilesAreIdentical(s,t) == (i == j));
    }
  }
  for (const std::string& filename: filenames)
  {
    fileio.DeleteFile(filename);
    BOOST_CHECK(!fileio.IsRegularFile(filename) && "Temporary file must be cleaned up");
  }
}

BOOST_AUTO_TEST_CASE(ribi_fileio_file_to_vector_and_vector_to_file_test)
{
  const ribi::FileIo fileio;
  //FileToVector and fileio.VectorToFile, indidual
  {
    const std::string filename = fileio.GetTempFileName();
    BOOST_CHECK(!fileio.IsRegularFile(filename));
    const std::vector<std::string> v { "A","BC", "DEF" };
    fileio.VectorToFile(v,filename);
    BOOST_CHECK(fileio.IsRegularFile(filename));
    const std::vector<std::string> w { fileio.FileToVector(filename) };
    BOOST_CHECK(v == w);
    fileio.DeleteFile(filename);
    BOOST_CHECK(!fileio.IsRegularFile(filename));
  }
  //FileToVector and fileio.VectorToFile, many
  {
    std::vector<std::vector<std::string>> v
    {
      { },
      { "A" },
      { "A", "B" },
      { "A", "B", "C" },
      { "A", "", "C" }
    };
    for (const std::vector<std::string>& w: v)
    {
      const std::string filename = fileio.GetTempFileName();
      BOOST_CHECK(!fileio.IsRegularFile(filename));
      fileio.VectorToFile(w,filename);
      BOOST_CHECK(fileio.IsRegularFile(filename));
      const std::vector<std::string> x { fileio.FileToVector(filename) };
      BOOST_CHECK(w == x);
      fileio.DeleteFile(filename);
      BOOST_CHECK(!fileio.IsRegularFile(filename));
    }
  }
}

BOOST_AUTO_TEST_CASE(ribi_fileio_folder_operations_test)
{
  const ribi::FileIo fileio;
  //Depth 1
  {
    const std::string f { fileio.GetTempFolderName() };
    BOOST_CHECK(!fileio.IsFolder(f) && "A temporary folder name must not exist");
    fileio.CreateFolder(f);
    BOOST_CHECK(fileio.IsFolder(f));
    fileio.DeleteFolder(f);
    BOOST_CHECK(!fileio.IsFolder(f) && "Temporary folder must be cleaned up");
  }
  //Depth 2
  {
    const std::string super_folder { fileio.GetTempFolderName() };
    BOOST_CHECK(!fileio.IsFolder(super_folder) && "A temporary folder name must not exist");
    fileio.CreateFolder(super_folder);
    BOOST_CHECK(fileio.IsFolder(super_folder));
    {
      const std::string f { fileio.GetTempFolderName() };
      BOOST_CHECK(!fileio.IsFolder(f) && "A temporary folder name must not exist");
      fileio.CreateFolder(f);
      BOOST_CHECK(fileio.IsFolder(f));
      fileio.DeleteFolder(f);
      BOOST_CHECK(!fileio.IsFolder(f));
    }
    BOOST_CHECK(fileio.IsFolder(super_folder));
    fileio.DeleteFolder(super_folder);
    BOOST_CHECK(!fileio.IsFolder(super_folder) && "Temporary folder must be cleaned up");
  }
}

BOOST_AUTO_TEST_CASE(ribi_fileio_get_file_basename_test)
{
  const ribi::FileIo fileio;
  BOOST_CHECK(fileio.GetFileBasename("") == "");
  BOOST_CHECK(fileio.GetFileBasename("tmp") == "tmp");
  BOOST_CHECK(fileio.GetFileBasename("tmp.") == "tmp");
  BOOST_CHECK(fileio.GetFileBasename("tmp.x") == "tmp");
  BOOST_CHECK(fileio.GetFileBasename("tmp.txt") == "tmp");
  BOOST_CHECK(fileio.GetFileBasename("tmp.text") == "tmp");
  BOOST_CHECK(fileio.GetFileBasename("tmp.longextension") == "tmp");
  BOOST_CHECK(fileio.GetFileBasename("input_triangle.txt") == "input_triangle");
  BOOST_CHECK(fileio.GetFileBasename("tmp") == "tmp");
  BOOST_CHECK(fileio.GetFileBasename("MyFolder/tmp") == "tmp");
  BOOST_CHECK(fileio.GetFileBasename("MyFolder/tmp.txt") == "tmp");
  BOOST_CHECK(fileio.GetFileBasename("MyFolder\\tmp.txt") == "tmp");
  BOOST_CHECK(fileio.GetFileBasename("MyFolder/MyFolder/tmp") == "tmp");
  BOOST_CHECK(fileio.GetFileBasename("MyFolder/MyFolder/tmp.txt") == "tmp");
  BOOST_CHECK(fileio.GetFileBasename("MyFolder/MyFolder\\tmp.txt") == "tmp");
  BOOST_CHECK(fileio.GetFileBasename("MyFolder/My-Folder\\tmp.txt") == "tmp");
  BOOST_CHECK(fileio.GetFileBasename("MyFolder/My_Folder\\tmp.txt") == "tmp");
  BOOST_CHECK(fileio.GetFileBasename("/home/richel/ProjectRichelBilderbeek/Games/GameConnectThree")
    == "GameConnectThree");
  BOOST_CHECK(fileio.GetFileBasename("/home/richel/ProjectRichelBilderbeek/Games/GameAminoAcidFighter")
    == "GameAminoAcidFighter");
  BOOST_CHECK(fileio.GetFileBasename("/home/richel/ProjectRichelBilderbeek/Games/GameK3OpEenRij")
    == "GameK3OpEenRij");
  BOOST_CHECK(fileio.GetFileBasename("/home/richel/ProjectRichelBilderbeek/Projects/ProjectRichelBilderbeek")
    == "ProjectRichelBilderbeek");
}

BOOST_AUTO_TEST_CASE(ribi_fileio_get_extension_test)
{
  const ribi::FileIo fileio;
  BOOST_CHECK(fileio.GetExtensionNoDot("") == "");
  BOOST_CHECK(fileio.GetExtensionNoDot("tmp") == "");
  BOOST_CHECK(fileio.GetExtensionNoDot("tmp.") == "");
  BOOST_CHECK(fileio.GetExtensionNoDot("tmp.x") == "x");
  BOOST_CHECK(fileio.GetExtensionNoDot("tmp.txt") == "txt");
  BOOST_CHECK(fileio.GetExtensionNoDot("tmp.text") == "text");
  BOOST_CHECK(fileio.GetExtensionNoDot("tmp.longextension") == "longextension");
  BOOST_CHECK(fileio.GetExtensionNoDot("input_triangle.txt") == "txt");
  BOOST_CHECK(fileio.GetExtensionNoDot("tmp") == "");
  BOOST_CHECK(fileio.GetExtensionNoDot("MyFolder/tmp") == "");
  BOOST_CHECK(fileio.GetExtensionNoDot("MyFolder/tmp.txt") == "txt");
  BOOST_CHECK(fileio.GetExtensionNoDot("MyFolder\\tmp.txt") == "txt");
  BOOST_CHECK(fileio.GetExtensionNoDot("MyFolder/MyFolder/tmp") == "");
  BOOST_CHECK(fileio.GetExtensionNoDot("MyFolder/MyFolder/tmp.txt") == "txt");
  BOOST_CHECK(fileio.GetExtensionNoDot("MyFolder/MyFolder\\tmp.txt") == "txt");
  BOOST_CHECK(fileio.GetExtensionNoDot("MyFolder/My-Folder\\tmp.txt") == "txt");
  BOOST_CHECK(fileio.GetExtensionNoDot("MyFolder/My_Folder\\tmp.txt") == "txt");

  BOOST_CHECK(fileio.GetExtensionWithDot("") == "");
  BOOST_CHECK(fileio.GetExtensionWithDot("tmp") == "");
  BOOST_CHECK(fileio.GetExtensionWithDot("tmp.") == ".");
  BOOST_CHECK(fileio.GetExtensionWithDot("tmp.x") == ".x");
  BOOST_CHECK(fileio.GetExtensionWithDot("tmp.txt") == ".txt");
  BOOST_CHECK(fileio.GetExtensionWithDot("tmp.text") == ".text");
  BOOST_CHECK(fileio.GetExtensionWithDot("tmp.longextension") == ".longextension");
  BOOST_CHECK(fileio.GetExtensionWithDot("input_triangle.txt") == ".txt");
  BOOST_CHECK(fileio.GetExtensionWithDot("tmp") == "");
  BOOST_CHECK(fileio.GetExtensionWithDot("MyFolder/tmp") == "");
  BOOST_CHECK(fileio.GetExtensionWithDot("MyFolder/tmp.txt") == ".txt");
  BOOST_CHECK(fileio.GetExtensionWithDot("MyFolder\\tmp.txt") == ".txt");
  BOOST_CHECK(fileio.GetExtensionWithDot("MyFolder/MyFolder/tmp") == "");
  BOOST_CHECK(fileio.GetExtensionWithDot("MyFolder/MyFolder/tmp.txt") == ".txt");
  BOOST_CHECK(fileio.GetExtensionWithDot("MyFolder/MyFolder\\tmp.txt") == ".txt");
  BOOST_CHECK(fileio.GetExtensionWithDot("MyFolder/My-Folder/tmp.txt") == ".txt");
  BOOST_CHECK(fileio.GetExtensionWithDot("MyFolder/My_Folder\\tmp.txt") == ".txt");

  BOOST_CHECK(fileio.GetExtension("") == "");
  BOOST_CHECK(fileio.GetExtension("tmp") == "");
  BOOST_CHECK(fileio.GetExtension("tmp.") == ".");
  BOOST_CHECK(fileio.GetExtension("tmp.x") == ".x");
  BOOST_CHECK(fileio.GetExtension("tmp.txt") == ".txt");
  BOOST_CHECK(fileio.GetExtension("tmp.text") == ".text");
  BOOST_CHECK(fileio.GetExtension("tmp.longextension") == ".longextension");
  BOOST_CHECK(fileio.GetExtension("input_triangle.txt") == ".txt");
  BOOST_CHECK(fileio.GetExtension("tmp") == "");
  BOOST_CHECK(fileio.GetExtension("MyFolder/tmp") == "");
  BOOST_CHECK(fileio.GetExtension("MyFolder/tmp.txt") == ".txt");
  BOOST_CHECK(fileio.GetExtension("MyFolder\\tmp.txt") == ".txt");
  BOOST_CHECK(fileio.GetExtension("MyFolder/MyFolder/tmp") == "");
  BOOST_CHECK(fileio.GetExtension("MyFolder/MyFolder/tmp.txt") == ".txt");
  BOOST_CHECK(fileio.GetExtension("MyFolder/MyFolder\\tmp.txt") == ".txt");
  BOOST_CHECK(fileio.GetExtension("MyFolder/My_Folder/tmp.txt") == ".txt");
  BOOST_CHECK(fileio.GetExtension("MyFolder/My-Folder\\tmp.txt") == ".txt");
}

BOOST_AUTO_TEST_CASE(ribi_fileio_get_files_in_folder_test)
{
  const ribi::FileIo fileio;
  //GetFilesInFolderRecursive
  {
    //Use the following structure:
    // - local.txt
    // - my_folder1/in_folder1.txt
    // - my_folder1/in_folder2.txt
    // - my_folder2/in_folder1.txt
    // - my_folder2/in_folder2.txt
    // - my_folder/my_subfolder/in_subfolder.txt
    const std::string local_filename { "local.txt" };
    const std::string folder_name1 { "my_folder1" };
    const std::string folder_name2 { "my_folder2" };
    const std::string in_folder_filename1 { "in_folder.txt1" };
    const std::string in_folder_filename2 { "in_folder.txt2" };
    const std::string subfolder_name1 { "my_subfolder1" };
    const std::string subfolder_name2 { "my_subfolder2" };
    const std::string in_subfolder_filename1 { "in_subfolder1.txt" };
    const std::string in_subfolder_filename2 { "in_subfolder2.txt" };
    //Folder creation
    for (const std::string& folder_name:
      {
        folder_name1,
        folder_name1 + fileio.GetPathSeperator() + subfolder_name1,
        folder_name1 + fileio.GetPathSeperator() + subfolder_name2,
        folder_name2,
        folder_name2 + fileio.GetPathSeperator() + subfolder_name1,
        folder_name2 + fileio.GetPathSeperator() + subfolder_name2,
      }
    )
    {
      if (!fileio.IsFolder(folder_name))
      {
        const std::string cmd = "mkdir " + folder_name;
        const int error = std::system(cmd.c_str());
        BOOST_CHECK(!error);
      }
      BOOST_CHECK(fileio.IsFolder(folder_name));
    }
    //File creation
    for (const std::string& filename:
      {
        local_filename,
        folder_name1 + fileio.GetPathSeperator() + in_folder_filename1,
        folder_name1 + fileio.GetPathSeperator() + in_folder_filename2,
        folder_name2 + fileio.GetPathSeperator() + in_folder_filename1,
        folder_name2 + fileio.GetPathSeperator() + in_folder_filename2,
        folder_name1 + fileio.GetPathSeperator() + subfolder_name1 + fileio.GetPathSeperator() + in_subfolder_filename1,
        folder_name1 + fileio.GetPathSeperator() + subfolder_name1 + fileio.GetPathSeperator() + in_subfolder_filename2,
        folder_name1 + fileio.GetPathSeperator() + subfolder_name2 + fileio.GetPathSeperator() + in_subfolder_filename1,
        folder_name1 + fileio.GetPathSeperator() + subfolder_name2 + fileio.GetPathSeperator() + in_subfolder_filename2,
        folder_name2 + fileio.GetPathSeperator() + subfolder_name1 + fileio.GetPathSeperator() + in_subfolder_filename1,
        folder_name2 + fileio.GetPathSeperator() + subfolder_name1 + fileio.GetPathSeperator() + in_subfolder_filename2,
        folder_name2 + fileio.GetPathSeperator() + subfolder_name2 + fileio.GetPathSeperator() + in_subfolder_filename1,
        folder_name2 + fileio.GetPathSeperator() + subfolder_name2 + fileio.GetPathSeperator() + in_subfolder_filename2,
      }
    )
    {
      if (!fileio.IsRegularFile(filename))
      {
        std::ofstream f(filename.c_str());
      }
      BOOST_CHECK(fileio.IsRegularFile(filename));
    }

    //Reading of the files and folders created
    const std::vector<std::string> v {
      fileio.GetFilesInFolderRecursive(folder_name1)
    };
    BOOST_CHECK(v.size() == 6);

    //Clean up
    fileio.DeleteFile(local_filename);
    fileio.DeleteFolder(folder_name1);
    fileio.DeleteFolder(folder_name2);
    BOOST_CHECK(!fileio.IsFolder(folder_name1) && "Temporary folder must be cleaned up");
    BOOST_CHECK(!fileio.IsFolder(folder_name2) && "Temporary folder must be cleaned up");
  }
}

BOOST_AUTO_TEST_CASE(ribi_fileio_get_path_test)
{
  const ribi::FileIo fileio;
  BOOST_CHECK(fileio.GetPath("/anypath/anyfile.cpp")=="/anypath");
  BOOST_CHECK(fileio.GetPath("/any-path/any-file.cpp")=="/any-path");
  BOOST_CHECK(fileio.GetPath("/any-path/any_file.cpp")=="/any-path");
  BOOST_CHECK(fileio.GetPath("/any_path/any-file.cpp")=="/any_path");
  BOOST_CHECK(fileio.GetPath("/any_path/any_file.cpp")=="/any_path");
  BOOST_CHECK(fileio.GetPath("C:\\any-path\\any-file.cpp")=="C:\\any-path");
  BOOST_CHECK(fileio.GetPath("C:\\any-path\\any_file.cpp")=="C:\\any-path");
  BOOST_CHECK(fileio.GetPath("C:\\any-path\\anyfile.cpp")=="C:\\any-path");
  BOOST_CHECK(fileio.GetPath("C:\\any_path\\any-file.cpp")=="C:\\any_path");
  BOOST_CHECK(fileio.GetPath("C:\\any_path\\any_file.cpp")=="C:\\any_path");
  BOOST_CHECK(fileio.GetPath("C:\\any_path\\anyfile.cpp")=="C:\\any_path");
  BOOST_CHECK(fileio.GetPath("C:\\anypath\\anyfile.cpp")=="C:\\anypath");
}

BOOST_AUTO_TEST_CASE(ribi_fileio_get_super_folder_test)
{
  const ribi::FileIo fileio;
  BOOST_CHECK(fileio.GetSuperFolder("C:\\A\\B")=="C:\\A");
  BOOST_CHECK(fileio.GetSuperFolder("C:\\A\\B\\C")=="C:\\A\\B");
  BOOST_CHECK(fileio.GetSuperFolder("C:\\")=="C:");
  BOOST_CHECK(fileio.GetSuperFolder("C:")=="");
  BOOST_CHECK(fileio.GetSuperFolder("/A/B")=="/A");
  BOOST_CHECK(fileio.GetSuperFolder("/A/B/C")=="/A/B");
  BOOST_CHECK(fileio.GetSuperFolder("/A")=="");
  BOOST_CHECK(fileio.GetSuperFolder("C:\\AnySuperFolder\\AnyFolder")=="C:\\AnySuperFolder");
  BOOST_CHECK(fileio.GetSuperFolder("C:\\AnySuperFolder\\Any_Folder")=="C:\\AnySuperFolder");
  BOOST_CHECK(fileio.GetSuperFolder("C:\\AnySuperFolder\\Any-Folder")=="C:\\AnySuperFolder");
  BOOST_CHECK(fileio.GetSuperFolder("C:\\Any_Super_Folder\\Any_Folder")=="C:\\Any_Super_Folder");
  BOOST_CHECK(fileio.GetSuperFolder("C:\\Any_Super_Folder\\Any-Folder")=="C:\\Any_Super_Folder");
  BOOST_CHECK(fileio.GetSuperFolder("C:\\Any-Super-Folder\\Any_Folder")=="C:\\Any-Super-Folder");
  BOOST_CHECK(fileio.GetSuperFolder("C:\\Any-Super-Folder\\Any-Folder")=="C:\\Any-Super-Folder");
  BOOST_CHECK(fileio.GetSuperFolder("/AnySuperFolder/AnyFolder")=="/AnySuperFolder");
  BOOST_CHECK(fileio.GetSuperFolder("/AnySuperFolder/Any_Folder")=="/AnySuperFolder");
  BOOST_CHECK(fileio.GetSuperFolder("/AnySuperFolder/Any-Folder")=="/AnySuperFolder");
  BOOST_CHECK(fileio.GetSuperFolder("/Any_Super_Folder/Any_Folder")=="/Any_Super_Folder");
  BOOST_CHECK(fileio.GetSuperFolder("/Any_Super_Folder/Any-Folder")=="/Any_Super_Folder");
  BOOST_CHECK(fileio.GetSuperFolder("/Any-Super-Folder/Any_Folder")=="/Any-Super-Folder");
  BOOST_CHECK(fileio.GetSuperFolder("/Any-Super-Folder/Any-Folder")=="/Any-Super-Folder");
}

BOOST_AUTO_TEST_CASE(ribi_fileio_is_regular_file_test)
{
  const ribi::FileIo fileio;
  const std::string filename { fileio.GetTempFileName() };

  //Create a regular file
  BOOST_CHECK(!fileio.IsRegularFile(filename));
  {
    std::fstream f;
    f.open(filename.c_str(),std::ios::out);
    f << "TEMP TEXT";
    f.close();
  }
  BOOST_CHECK(fileio.IsRegularFile(filename));

  fileio.DeleteFile(filename);

  BOOST_CHECK(!fileio.IsRegularFile(filename) && "Temporary file must be cleaned up");
}

BOOST_AUTO_TEST_CASE(ribi_fileio_remove_path_test)
{
  const ribi::FileIo fileio;
  BOOST_CHECK(fileio.RemovePath("x.txt") == "x.txt");
  BOOST_CHECK(fileio.RemovePath("MyFolder/x.txt") == "x.txt");
  BOOST_CHECK(fileio.RemovePath("Another/MyFolder/x.txt") == "x.txt");
  BOOST_CHECK(fileio.RemovePath("Yet/Another/MyFolder/x.txt") == "x.txt");
  BOOST_CHECK(fileio.RemovePath("MyFolder\\x.txt") == "x.txt");
  BOOST_CHECK(fileio.RemovePath("Another\\MyFolder\\x.txt") == "x.txt");
  BOOST_CHECK(fileio.RemovePath("Yet\\Another\\MyFolder\\x.txt") == "x.txt");
  BOOST_CHECK(fileio.RemovePath("Another/MyFolder\\x.txt") == "x.txt");
  BOOST_CHECK(fileio.RemovePath("Another\\MyFolder/x.txt") == "x.txt");
  BOOST_CHECK(fileio.RemovePath("Yet\\Another/MyFolder\\x.txt") == "x.txt");
  BOOST_CHECK(fileio.RemovePath("Yet\\Another\\MyFolder/x.txt") == "x.txt");
  BOOST_CHECK(fileio.RemovePath("Yet\\An-other\\MyFolder/x.txt") == "x.txt");
  BOOST_CHECK(fileio.RemovePath("Yet\\An_other\\MyFolder/x.txt") == "x.txt");
  BOOST_CHECK(fileio.RemovePath("Yet\\Another\\MyFolder/x_y.txt") == "x_y.txt");
  BOOST_CHECK(fileio.RemovePath("Yet\\Another\\MyFolder/x-y.txt") == "x-y.txt");
}

BOOST_AUTO_TEST_CASE(ribi_fileio_rename_file_test)
{
  const ribi::FileIo fileio;
  const std::string filename_from { fileio.GetTempFileName() };
  const std::string filename_to   { fileio.GetTempFileName() };

  BOOST_CHECK(!fileio.IsRegularFile(filename_from));
  BOOST_CHECK(!fileio.IsRegularFile(filename_to));

  //Create new file
  {
    std::ofstream f(filename_from.c_str());
    f << "TMP";
    f.close();
  }

  //Only filename_from will exist
  BOOST_CHECK( fileio.IsRegularFile(filename_from));
  BOOST_CHECK(!fileio.IsRegularFile(filename_to));

  //Rename
  fileio.RenameFile(filename_from,filename_to);

  //Only filename_to will exist
  BOOST_CHECK(!fileio.IsRegularFile(filename_from));
  BOOST_CHECK( fileio.IsRegularFile(filename_to));

  //Clean up
  fileio.DeleteFile(filename_to);
  BOOST_CHECK(!fileio.IsRegularFile(filename_to) && "Temporary file must be cleaned up");
}

BOOST_AUTO_TEST_CASE(ribi_fileio_simplify_path_test)
{
  const ribi::FileIo fileio;
  BOOST_CHECK(fileio.SimplifyPath("/home/richel/Projects/Tools/ToolTestProFile/../../Classes/CppQtAboutDialog/qtaboutdialog.ui")
    == "/home/richel/Projects/Classes/CppQtAboutDialog/qtaboutdialog.ui");
  BOOST_CHECK(fileio.SimplifyPath("\\home\\richel\\Projects\\Tools\\ToolTestProFile\\..\\..\\Classes\\CppQtAboutDialog\\qtaboutdialog.ui")
    == "\\home\\richel\\Projects\\Classes\\CppQtAboutDialog\\qtaboutdialog.ui");
  BOOST_CHECK(fileio.SimplifyPath("/home/richel/Projects/Tools/../Classes/CppQtAboutDialog/qtaboutdialog.ui")
    == "/home/richel/Projects/Classes/CppQtAboutDialog/qtaboutdialog.ui");
  BOOST_CHECK(fileio.SimplifyPath("\\home\\richel\\Projects\\Tools\\..\\Classes\\CppQtAboutDialog\\qtaboutdialog.ui")
    == "\\home\\richel\\Projects\\Classes\\CppQtAboutDialog\\qtaboutdialog.ui");
  BOOST_CHECK(fileio.SimplifyPath("/home/richel/Projects/Classes/CppQtAboutDialog/qtaboutdialog.ui")
    == "/home/richel/Projects/Classes/CppQtAboutDialog/qtaboutdialog.ui");
  BOOST_CHECK(fileio.SimplifyPath("\\home\\richel\\Projects\\Classes\\CppQtAboutDialog\\qtaboutdialog.ui")
    == "\\home\\richel\\Projects\\Classes\\CppQtAboutDialog\\qtaboutdialog.ui");
  BOOST_CHECK(fileio.SimplifyPath("Tools/..") == "");
  BOOST_CHECK(fileio.SimplifyPath("Tools\\..") == "");
  BOOST_CHECK(fileio.SimplifyPath("Tools/../A") == "/A");
  BOOST_CHECK(fileio.SimplifyPath("Tools\\..\\A") == "\\A");
  BOOST_CHECK(fileio.SimplifyPath("Tools/../A/B") == "/A/B");
  BOOST_CHECK(fileio.SimplifyPath("Tools\\..\\A\\B") == "\\A\\B");
  BOOST_CHECK(fileio.SimplifyPath("../../Tools/ToolCreateQtProjectZipFile/../../Libraries/Boost.pri")
    == "../../Libraries/Boost.pri");
  BOOST_CHECK(fileio.SimplifyPath("..\\..\\Tools\\ToolCreateQtProjectZipFile\\..\\..\\Libraries\\Boost.pri")
    == "..\\..\\Libraries\\Boost.pri");
}

BOOST_AUTO_TEST_CASE(ribi_fileio_has_right_path_seperators_test)
{
  const ribi::FileIo fileio;
  #ifdef _WIN32
  BOOST_CHECK( fileio.HasRightPathSeperators("\\A\\B\\text.txt"));
  BOOST_CHECK(!fileio.HasRightPathSeperators("/A/B/text.txt"));
  #else
  BOOST_CHECK(!fileio.HasRightPathSeperators("\\A\\B\\text.txt"));
  BOOST_CHECK( fileio.HasRightPathSeperators("/A/B/text.txt"));
  #endif
}

BOOST_AUTO_TEST_CASE(ribi_fileio_has_two_path_seperators_test)
{
  const ribi::FileIo fileio;
  //HasTwoPathSeperators
  {
    BOOST_CHECK(!fileio.HasTwoPathSeperators("\\A\\B\\text.txt"));
    BOOST_CHECK(!fileio.HasTwoPathSeperators("/A/B/text.txt"));

    BOOST_CHECK( fileio.HasTwoPathSeperators("/A\\B/text.txt"));
    BOOST_CHECK( fileio.HasTwoPathSeperators("\\A/B/text.txt"));
    BOOST_CHECK( fileio.HasTwoPathSeperators("/A/B\\text.txt"));
    BOOST_CHECK( fileio.HasTwoPathSeperators("/A\\B\\text.txt"));
    BOOST_CHECK( fileio.HasTwoPathSeperators("\\A\\B/text.txt"));
    BOOST_CHECK( fileio.HasTwoPathSeperators("\\A/B\\text.txt"));
  }
}

BOOST_AUTO_TEST_CASE(ribi_fileio_is_unix_path_test)
{
  const ribi::FileIo fileio;
  //IsUnixPath
  {
    BOOST_CHECK(!fileio.IsUnixPath("\\A\\B\\text.txt"));
    BOOST_CHECK( fileio.IsUnixPath("/A/B/text.txt"));
    BOOST_CHECK(!fileio.IsUnixPath("/A\\B/text.txt"));
    BOOST_CHECK(!fileio.IsUnixPath("\\A/B/text.txt"));
    BOOST_CHECK(!fileio.IsUnixPath("/A/B\\text.txt"));
    BOOST_CHECK(!fileio.IsUnixPath("/A\\B\\text.txt"));
    BOOST_CHECK(!fileio.IsUnixPath("\\A\\B/text.txt"));
    BOOST_CHECK(!fileio.IsUnixPath("\\A/B\\text.txt"));
  }

}

BOOST_AUTO_TEST_CASE(ribi_fileio_convert_path_to_unix_test)
{
  const ribi::FileIo fileio;
  BOOST_CHECK(fileio.IsUnixPath(fileio.ConvertPathToUnix("\\A\\B\\text.txt")));
  BOOST_CHECK(fileio.IsUnixPath(fileio.ConvertPathToUnix("/A/B/text.txt")));
  BOOST_CHECK(fileio.IsUnixPath(fileio.ConvertPathToUnix("/A\\B/text.txt")));
  BOOST_CHECK(fileio.IsUnixPath(fileio.ConvertPathToUnix("\\A/B/text.txt")));
  BOOST_CHECK(fileio.IsUnixPath(fileio.ConvertPathToUnix("/A/B\\text.txt")));
  BOOST_CHECK(fileio.IsUnixPath(fileio.ConvertPathToUnix("/A\\B\\text.txt")));
  BOOST_CHECK(fileio.IsUnixPath(fileio.ConvertPathToUnix("\\A\\B/text.txt")));
  BOOST_CHECK(fileio.IsUnixPath(fileio.ConvertPathToUnix("\\A/B\\text.txt")));
}
