How To Use Auto File Backup:

AutoFile Backup takes a list of directories contained in FileList.txt and copies them to the directory contained in config.txt under the option "CopyDirectory"

The programs prompts for new directories everytime it is run, these can be pasted into the console (or typed out, if your one of those people)
The FileList.txt file can also be edited directly, directories need to be entered one per line with no preceding or following spaces


AutoFilePath checks the validity of direcory paths as they are added (i.e. do they exist), if this is done in console. If you get this error for a valid filepath then the program most likely does not have access to that directory and needs escalated privileges ect


all directories need to be full file paths


AutoFileBackup follows the following rules:

At this time AutoBackup only produces one version of each file in the copied directories, which is updated every time the program is run.
directories are copied to CopyDirectory, starting with the first directory in FileList
Each directory is copy to a folder in the copy directory of the same name as the parent folder of the directory
all files and folders in the directory are copied (with the below exceptions), AutoFileBackup maintains the folder structure of the original directory in the relevent folder.

To save CPU time AutoFile Backup compares the file last modified date to the modified date of the file in the backup directory, if these are equal, the file is assumed to be identical and the file is not copied.
If for whatever reason the file in the copy directory is newer than the file being copied to it, an overwrite prompt will be displayed on the programs output console.

- in windows 8 and above, pointing the CopyDirectory to a onedrive folder will cause the directory to be synced to the cloud

- The task scheduler can be used to run this application on a regular basis