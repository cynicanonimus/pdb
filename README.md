pdb
===
PDB is  cross-platform client-server personal information manager. 
Closest analog is TreePad.

Client side - Qt.
Server side - SQL server (Right now tested with MySql, Oracle, SQLLite).

Product features: 

1. Save and manage unlimited number of documents and trees of documents. 
2. Support transparent documents encryption
3. Powerfull search for documents. 
4. Support SSH-tunnels to the database. 
5. Build-in text editor. 
6. Import directories structure with all files, export tree of documents as directories structure. 
7. Scan documents from the application and put it into DB directly.


How to build the project: 

1. Download all sources (include AdvThreadPool from https://github.com/cynicanonimus/AdvThreadPool !)
2. Launch QtCreator, open file AdvThreadPool.pro. 
3. Launch QtCreator, open file pdb/pdb.pro. Make full rebuild. 
4. Launch QtCreator, open file PdbSettings/ PdbSettings.pro Make full rebuild. 
5. Copy executable files pdb and PdbSettings and file dbdescription.xml into same directory. 
6. Check  dbdescription.xml. If your database is not described, describe it once and let me know. :) 
7. Launch  PdbSettings, make all settings ("Database", "color and application style"... etc)
8. Do not forget to click "Apply changes"
9. Check if you have drivers for your database. 
10. For Linux: put path to your database driver in $PATH, or put your database driver into known path 
11. Launch executable file pdb. 
12. Enjoy. :)
