
#ifndef __HEALTHOBJ_H_
#define __HEALTHOBJ_H_
  
    /*!
    * \brief This object modifies an entity's hit points
    *
    * The purpose of this object is to hang around in your game world
    * until the Player picks it up. Depending upon how much health 
    * points the instance of this object is worth, it is added or
    * subtracted to the player's overall health score
    */
    class HealthObj : public ObjA
    {
      public:
      /*!
      * constructor
      */
      HealthObj();

      /*!
      * destructor
      */
      virtual ~HealthObj();

      //comment blocks can also begin with...
      /**
      * This method places our object in the game world along
      * with assigning it a health value
      * @param x - x position
      * @param y - y position
      * @param z - z position
      * @param h - health
      * @return true or false if this object was allowed
      */
      bool setPosition( float x, float y, float z, int h );

      //snip!
    };

#endif
