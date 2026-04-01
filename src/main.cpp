/**
*@file main.cpp
*@author Ken, Zach, Gabriel, Robert
*@date 2026-02-10
*@brief main function for the Zip Code Analyzer project, which initializes the application and processes user input.
 */

#include <iostream>
#include <fstream>
#include "../include/analyzer.h"
#include "../include/ZipCodeBuffer.h"
#include "../include/Printer.h"
#include "../include/PrinterCSV.h"

void test_function ()
{
    cout << "\nTesting Buffer"<< endl;
    BlockedBuffer B_Buffer
    
}







int main() {

    Analyzer analyzer;
    ZipCodeBuffer buffer("data/us_postal_codes.csv");
    Printer printer;
    PrinterCSV printerCSV;
    ZipCodeRecord r;

    std::ofstream outFile1("Output1.csv");
    std::ofstream outFile2("Output2.csv");

    
    while(buffer.readNext(r))
    {
        analyzer.consume(r);
    }

    Analyzer analyzer2;
    ZipCodeBuffer buffer2("data/us_postal_codes_ROWS_RANDOMIZED.csv");
    Printer printer2;
    PrinterCSV printerCSV2;
    ZipCodeRecord r2;

    
    while(buffer2.readNext(r2))
    {
        analyzer2.consume(r2);
    }
        
   printerCSV.printCSV(outFile1, analyzer.getResults());
   printerCSV2.printCSV(outFile2, analyzer2.getResults());

    outFile1.close();
    outFile2.close();



// Instructions:
//      Your blocked sequence set generation program's command line options should include:
//          - The name of the blocked sequence set data file
//          - All other information necessary for the header file


//      Process sequentially a blocked sequence set file using buffer classes. {functionality from Group Projects 1 & 2}

    
    //unpacks a record from a block into a record buffer
    BlockBuffer B_Buffer(//"block sequence file")
    
    //unpacks fields from record buffer into record object
    Length_Indicated_ZipCodeBuffer Z_Buffer(//record buffer//)

    while(B_buffer.readNext(/*placeholder*/)





    return 0;
}

        