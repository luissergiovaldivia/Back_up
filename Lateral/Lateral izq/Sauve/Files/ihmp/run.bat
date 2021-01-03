ECHO *************** Mise à jour d'un MOP ****************************************

ECHO *************** Setup Data Collector ************************************
NET STOP IhmpDataCollector
RD "C:\ihmp\Assemblies\Serveur" /S /Q
MD "C:\ihmp\Assemblies\Serveur"
xcopy "C:\TempMaj\Assemblies\Serveur\*.*" "c:\Ihmp\Assemblies\Serveur" /E /K /H /Y

tempo.vbs

ECHO *************** Setup IHMP+ *********************************************
RD "C:\ihmp\Assemblies\Clients" /S /Q
RD "C:\ihmp\Assemblies\Ihmp" /S /Q
RD "C:\ihmp\Assemblies\OdilEx" /S /Q
RD "C:\ihmp\XMLData\odil_data" /S/Q
RD "C:\ihmp\XMLData\common_data" /S/Q
MD "C:\ihmp\Assemblies\Clients"
MD "C:\ihmp\Assemblies\Ihmp"
MD "C:\ihmp\Assemblies\OdilEx"
MD "C:\ihmp\XMLData\Odil_data"
MD "C:\ihmp\XMLData\Common_data"
xcopy "C:\TempMaj\Assemblies\Clients\*.*" "c:\Ihmp\Assemblies\Clients" /E /I /Y
xcopy "C:\TempMaj\Assemblies\OdilEx\*.*" "c:\Ihmp\Assemblies\OdilEx" /E /I /Y
xcopy "C:\TempMaj\XMLData" c:\Ihmp\XMLData\ /E /I /Y

ECHO ** redémarrer le collecteur après la mise à jour du standard (qui contient les assemblies ihmp)
NET START IhmpDataCollector

ECHO *************** SetupIhmpPanel ***************
xcopy "C:\TempMaj\PupitreIhmp" c:\PupitreIhmp /E /I /Y

ECHO *************** Mise à jour FTP_DATA ***************
xcopy "C:\TempMaj\Ftp_data\*.*" "c:\ihmp\XMLData\Ftp_data" /E /I /Y
DEL "c:\Ihmp\XMLData\Ftp_data\IhmpRenault.dll" /F /Q
DEL "c:\Ihmp\XMLData\Ftp_data\IHMPRenault.maj" /F /Q

ECHO *************** Mise à jour SaveRestit ***************
xcopy "C:\TempMaj\PupitreSiemens\*.*" "c:\PupitreSiemens" /E /I /Y

ECHO ********* Suppression raccourci ODIL(Administrateur) *********
DEL "C:\Documents and Settings\Administrator\Start Menu\3. Configuration-Diagnostic\05 Configuration IHMP-SMPLOC\06 ODIL (Administrator).lnk"
DEL "C:\Documents and Settings\All Users\Start Menu\3. Configuration-Diagnostic\05 Configuration IHMP-SMPLOC\06 ODIL (Administrator).lnk"
DEL "C:\Documents and Settings\Exploitation\Start Menu\3. Configuration-Diagnostic\05 Configuration IHMP-SMPLOC\06 ODIL (Administrator).lnk"