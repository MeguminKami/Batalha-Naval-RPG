#include "character.h"


void character::loadCharacter()
{

    // Builds the passive
    passive.name = "Looting";
    passive.description = "At the end of the turn if a enemy ship was destroyed you claim the loot having 1 extra action point in the next turn.";

    // Builds the primary skill
    primary.name = "Air strike";
    primary.description = "Strikes and air strike that damages everything in a 1x3 line.";
    primary.setCost(3);
    primary.setAoeSize(3);
    // AOE positions centered in a 0,0
    for(int i = 0; i < primary.getAoeSize(); i++) primary.setAoePos({0,i});

    // Builds the secondary skill
    secondary.name = "Lucky Missile";
    secondary.description = "Shots a random missile that destroys if it hits.";
    secondary.setCost(3);
    secondary.setAoeSize(1);
    secondary.setAoePos({0,0});

    // Builds the ultimate skill
    ultimate.name = "Nuclear";
    ultimate.description = "Shots a nuclear bomb that damages everything in 3x3 square";
    ultimate.setCost(6);
    ultimate.setAoeSize(9);
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++) ultimate.setAoePos({i,j});
    }

}

void character::usePrimary()
{

}
