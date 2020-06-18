/* paste this code below this in game.cpp:

    if(!canThrowObjectTo(mapFromPos, mapToPos) && !player->hasCustomFlag(PlayerCustomFlag_CanThrowAnywhere)) {
        player->sendCancelMessage(RET_CANNOTTHROW);
        return false;
    }

*/

    uint16_t items[] = {2148, 2152, 2160, 3976, 2599};
    uint16_t n = 0;
    for (n = 0; n < sizeof(items) / sizeof(uint16_t); n++) {
        if(item->getID() == items[n] && player->hasCondition(CONDITION_EXHAUST, 1)) {
            player->sendTextMessage(MSG_STATUS_SMALL, "Please wait a few seconds to move this item.");
            return false;
        }
    }

    if(Condition* condition = Condition::createCondition(CONDITIONID_DEFAULT, CONDITION_EXHAUST, 1000, 0, false, 1)) {
        player->addCondition(condition);
    }
