#ifndef CHARACTER_H
#define CHARACTER_H
#include "skill.h"
#include "passive.h"
#include "preview.h"

#define CAPTAIN "Captain"
#define CAPTAIN_ID 1

#define PIRATE "Pirate"
#define PIRATE_ID 2

#define SPACE "Space Commander"
#define SPACE_ID 3

#define RESET_HERO -1

class character
{
public:
    // Character name
    QString name;

    // Preview for the skills
    preview preview;

    // Skills
    skill normal;
    skill primary;
    skill secondary;
    skill ultimate;
    passive passive;

    // Loads the character details
    void loadCharacter(int ID);

};

#endif // CHARACTER_H
