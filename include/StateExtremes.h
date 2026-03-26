/**
*@file StateExtremes.h
*@author Robert Shores & Zach Houck
*@date 2026-02-12
*@brief Header file for the StateExtremes class, which provides methods for analyzing data sets.
 */

#ifndef STATEEXTREMES_H
#define STATEEXTREMES_H


#include "ZipCodeRecord.h"
 
/** Option A : Data only struct - This just stores records and does not have independent behavior. 
    

*/

struct StateExtremes
{
    /** @brief True once this state has at least one record. */
    bool initialized = false;

    /** @brief ZipCodeRecord with least longitude for this state. */
    ZipCodeRecord easternmost;

    /** @brief ZipCodeRecord with greatest longitude for this state. */
    ZipCodeRecord westernmost;

    /** @brief ZipCodeRecord with greatest latitude for this state. */
    ZipCodeRecord northernmost;

    /** @brief ZipCodeRecord with least latitude for this state. */
    ZipCodeRecord southernmost;
};



/** Option B : Object Oriented Approach
    State Extremes becomes an encapsulated class

*/
// class StateExtremes

//    {
//       private: 

//          /**
//          * @brief initialized is false until the first record is consumed, then it is set to true and the min and max values are updated accordingly
//          */
//          bool initialized = false;

//          /**
//          * @brief Stores a ZipCodeRecord for all of the directions extremes
//          */
//          ZipCodeRecord easternmost;
//          ZipCodeRecord westernmost;
//          ZipCodeRecord northernmost;
//          ZipCodeRecord southernmost;
      
//       public:

//       /** 
//       * @brief Default Constructor
//       * @post creates an uninitialized StateExtremes object
//       */
//       StateExtremes();

//       /** 
//       * @brief Constructor
//       * @param FirstZip the ZipCodeRecord used to create a StateExtremes object
//       * @post creates an initialized StateExtremes object
//       */
//       StateExtremes(ZipCodeRecord FirstZip);

//       /** 
//       * @brief Get functions
//       */
//       bool isInitialized();
//       ZipCodeRecord getEastern();
//       ZipCodeRecord getWestern();
//       ZipCodeRecord getNorthern();
//       ZipCodeRecord getSouthern();

//       /** 
//       * @brief Set functions
//       */
//       void setInitialized(bool init);
//       void setEastern(ZipCodeRecord zip);
//       void setWestern(ZipCodeRecord zip);
//       void setNorthern(ZipCodeRecord zip);
//       void setSouthern(ZipCodeRecord zip);
//    };






#endif // STATE_EXTREMES_H