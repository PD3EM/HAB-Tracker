/* 
*
* part of the code from the Apex III Project
* http://www.apexhab.org/apex-iii/
*
* Priyesh Patel
*
*/

void sdcard_log(char* sentence)
{
    File logFile = SD.open(SD_LOG_FILENAME, FILE_WRITE);
    
    if (logFile)
    {
        logFile.println(sentence);
    }

    logFile.close();
}
