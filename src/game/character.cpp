#include "character.h"


void character::loadCharacter(int ID)
{
    normal.cooldown = 0;
    primary.cooldown = 0;
    secondary.cooldown = 0;
    ultimate.cooldown = 0;


    switch (ID) {
        case CAPTAIN_ID:
        // Name
        name = CAPTAIN;

        // Builds the passive
        passive.name = "Radar";
        passive.description = "The Captain fights with firepower, but his unique passive transforms his normal skill into a Radar that allows him to scan the enemy field.";

        // Builds the normal skill
        normal.name = "Radar";
        normal.description = "Uses the radar to scan a 2x2 square. If the radar dectects any enemy ships that square will be highlighted. Costs 2 action points.";
        normal.setCost(2);
        normal.setAoeSize({2,2});
        normal.used = false;
        normal.placed = false;
        normal.destroy = false;

        // Builds the primary skill
        primary.name = "Air Strike";
        primary.description = "An aerial attack will damage everything in a 1x3 line. Costs 2 action points.";
        primary.setCost(2);
        primary.setAoeSize({1,3});
        primary.used = false;
        primary.placed = false;
        primary.destroy = false;

        // Builds the secondary skill
        secondary.name = "Torpedo";
        secondary.description = "Fires a torpedo that will destroy the target if it hits it. Costs 3 action points.";
        secondary.setCost(3);
        secondary.setAoeSize({1,1});
        secondary.used = false;
        secondary.placed = false;
        secondary.destroy = true;

        // Builds the ultimate skill
        ultimate.name = "Nuclear";
        ultimate.description = "Launch an atomic bomb that damages everything in a 3x3 square. Costs 6 action points.";
        ultimate.setCost(6);
        ultimate.setAoeSize({3,3});
        ultimate.used = false;
        ultimate.placed = false;
        ultimate.destroy = false;

        break;

        case PIRATE_ID:
        // Name
        name = PIRATE;

        // Builds the passive
        passive.name = "Looting";
        passive.description = "Whenever he destroys an enemy ship he claims the loot having 1 extra action point in the next turn.";

        // Builds the normal skill
        normal.name = "Cannon Ball";
        normal.description = "Uses 2 action points to shoot a square.";
        normal.setCost(2);
        normal.setAoeSize({1,1});
        normal.used = false;
        normal.placed = false;
        normal.destroy = false;


        // Builds the primary skill
        primary.name = "Cannon Barrage";
        primary.description = "Shoots a 1x4 line of cannon balls. Costs 3 action points.";
        primary.setCost(3);
        primary.setAoeSize({1,4});
        primary.used = false;
        primary.placed = false;
        primary.destroy = false;

        // Builds the secondary skill
        secondary.name = "Explosive Barel";
        secondary.description = "You shoot a barel, which explodes in a 2x2 area. Costs 2 action points.";
        secondary.setCost(2);
        secondary.setAoeSize({2,2});
        secondary.used = false;
        secondary.placed = false;
        secondary.destroy = false;

        // Builds the ultimate skill
        ultimate.name = "Humungous!";
        ultimate.description = "Destroy an enemy ship by hitting a damaged square with the humungous cannon. Costs 5 action points.";
        ultimate.setCost(5);
        ultimate.setAoeSize({1,1});
        ultimate.used = false;
        ultimate.placed = false;
        ultimate.destroy = true;

        break;

        case SPACE_ID:
        // Name
        name = SPACE;

        // Builds the passive
        passive.name = "Phaser";
        passive.description = "Using advanced energy recovery tecnology his normal shot uses only 1 action point and he can shoot again if he hits.";

        // Builds the normal skill
        normal.name = "Phaser Weapon";
        normal.description = "Uses 1 action point to laser a square if this laser hits you can use it again.";
        normal.setCost(1);
        normal.setAoeSize({1,1});
        normal.used = false;
        normal.placed = false;
        normal.destroy = false;

        // Builds the primary skill
        primary.name = "Ion Blaster";
        primary.description = "Fires a blaster in a diagonal line that will continue until it hits a target. Costs 4 action points.";
        primary.setCost(4);
        primary.setAoeSize({1,1});
        primary.used = false;
        primary.placed = false;
        primary.destroy = false;

        // Builds the secondary skill
        secondary.name = "Double Beam";
        secondary.description = "Shot a double-beam that will cause damage 2 squares. Costs 2 action points.";
        secondary.setCost(2);
        secondary.setAoeSize({1,2});
        secondary.used = false;
        secondary.placed = false;
        secondary.destroy = false;

        // Builds the ultimate skill
        ultimate.name = "Superlaser";
        ultimate.description = "Fires a full-star laser that will do damage to anything in a 1x12 line. Costs 8 action points.";
        ultimate.setCost(8);
        ultimate.setAoeSize({1,12});
        ultimate.used = false;
        ultimate.placed = false;
        ultimate.destroy = false;

        break;

        case RESET_HERO:
            // Name
            name ="";

            // Builds the passive
            passive.name = "";
            passive.description = "";

            // Builds the normal skill
            normal.name = "";
            normal.setCost(0);
            normal.setAoeSize({0,0});
            normal.used = false;
            normal.placed = false;
            normal.destroy = false;

            // Builds the primary skill
            primary.name = "";
            primary.description = "";
            primary.setCost(0);
            primary.setAoeSize({0,0});
            primary.used = false;
            primary.placed = false;
            primary.destroy = false;

            // Builds the secondary skill
            secondary.name = "";
            secondary.description = "";
            secondary.setCost(0);
            secondary.setAoeSize({0,0});
            secondary.used = false;
            secondary.placed = false;
            secondary.destroy = false;

            // Builds the ultimate skill
            ultimate.name = "";
            ultimate.description = "";
            ultimate.setCost(0);
            ultimate.setAoeSize({0,0});
            ultimate.used = false;
            ultimate.placed = false;
            ultimate.destroy = false;
        break;

        default:
        break;
    }

}



