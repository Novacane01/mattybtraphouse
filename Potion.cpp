#include "stdafx.h"
#include "Potion.h"
#include "Player.h"

void HealthPotion::Heal(Player *player) {
    player->setHp(100);
}

//void SpeedPotion::Speed(Player *player) {
//    player->setWalkSpeed(1.5 * player->getWalkspeed());
//    for(Weapon i:player->weaponInventory){
//        i.setAttackSpeed(1.5 * i.getAttackSpeed());
//    }
//}

void StaminaPotion::Stamina(Player *player) {

}

//void AttackPotion::increaseAttack(Player *player) {
//    for(Weapon i:player->weaponInventory){
//        i.setDamage(1.5 * i.getDamage());
//    }
//}

void TimePotion::slowTime() {

}

