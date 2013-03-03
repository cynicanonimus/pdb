pdb
===
PDB is  cross-platform client-server personal information manager. 
Closest analog is TreePad.

Client side - Qt.
Server side - any SQL server.

How to build the project: 

1. Download all sources. 
2. Launch QtCreator, open file pdb/pdb.pro. Make full rebuild. 
3. Launch QtCreator, open file PdbSettings/ PdbSettings.pro Make full rebuild. 
4. Copy executable files pdb and PdbSettings and file dbdescription.xml into same directory. 
5. Check  dbdescription.xml. If your database is not described, describe it once and let me know. :) 
6. Launch  PdbSettings, make all settings ("Database", "color and application style"... etc)
7. Do not forget to click "Apply changes"
8. Check if you have drivers for your database. 
9. For Linux: put path to your database driver in $PATH, or put your database driver into known path 
10. Launch executable file  pdb. 
