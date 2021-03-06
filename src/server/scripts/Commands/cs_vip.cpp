/*
* Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "ScriptMgr.h"
#include "Chat.h"
#include "Common.h"
#include "Player.h"
#include "WorldSession.h"
#include "QuantumSystemText.h"
#include "Log.h"
#include "SpellAuras.h"
#include "World.h"
#include "Config.h"

class vip_commandscript : public CommandScript
{
public:
    vip_commandscript() : CommandScript("vip_commandscript") { }

    ChatCommand* GetCommands() const
    {
        static ChatCommand vipCommandTable[] =
        {
            { "debuff",         SEC_PLAYER,         false, &HandleVipDebuffCommand,             "", NULL },
            { "bank",           SEC_PLAYER,         false, &HandleVipBankCommand,               "", NULL },
            { "repair",         SEC_PLAYER,         false, &HandleVipRepairCommand,             "", NULL },
            { "resettalents",   SEC_PLAYER,         false, &HandleVipResetTalentsCommand,       "", NULL },
            { "taxi",           SEC_PLAYER,         false, &HandleVipTaxiCommand,               "", NULL },
            { "home",           SEC_PLAYER,         false, &HandleVipHomeCommand,               "", NULL },
            { "capital",        SEC_PLAYER,         false, &HandleVipCapitalCommand,            "", NULL },
            { NULL,             0,                  false, NULL,                                "", NULL }
        };
        static ChatCommand commandTable[] =
        {
			{ "vip",            SEC_PLAYER,         false, NULL,                                "", vipCommandTable },
			{ NULL,             0,                  false, NULL,                                "", NULL }
        };
        return commandTable;
    }

    static bool HandleVipDebuffCommand(ChatHandler* handler, const char* /*args*/)
    {   
        Player* character = handler->GetSession()->GetPlayer();

        if (!handler->GetSession()->IsPremiumAccount())
        {
            handler->SendSysMessage(NOT_VIP);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!sWorld->getBoolConfig(CONFIG_VIP_DEBUFF_COMMAND))
        {
            handler->SendSysMessage(VIP_COMMAND_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (character->IsInCombatActive() || character->IsInFlight() || character->GetMap()->IsBattlegroundOrArena() || character->HasStealthAura() || character->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || character->IsDead())
        {
            handler->SendSysMessage(VIP_CANT_DO);
            handler->SetSentErrorMessage(true);
            return false;
        }

        handler->GetSession()->GetPlayer()->RemoveAurasDueToSpell(15007);
        handler->GetSession()->GetPlayer()->RemoveAurasDueToSpell(26013);
        return true;
    }

    static bool HandleVipBankCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* character = handler->GetSession()->GetPlayer();

        if (!handler->GetSession()->IsPremiumAccount())
        {
            handler->SendSysMessage(NOT_VIP);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!sWorld->getBoolConfig(CONFIG_VIP_BANK_COMMAND))
        {
            handler->SendSysMessage(VIP_COMMAND_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (character->IsInCombatActive() || character->IsInFlight() || character->GetMap()->IsBattlegroundOrArena() || character->HasStealthAura() || character->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || character->IsDead())
        {
            handler->SendSysMessage(VIP_CANT_DO);
            handler->SetSentErrorMessage(true);
            return false;
        }

        handler->GetSession()->SendShowBank(handler->GetSession()->GetPlayer()->GetGUID());

        return true;
    }

    static bool HandleVipRepairCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* character = handler->GetSession()->GetPlayer();

        if (!handler->GetSession()->IsPremiumAccount())
        {
            handler->SendSysMessage(NOT_VIP);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!sWorld->getBoolConfig(CONFIG_VIP_REPAIR_COMMAND))
        {
            handler->SendSysMessage(VIP_COMMAND_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (character->IsInCombatActive() || character->IsInFlight() || character->GetMap()->IsBattlegroundOrArena() || character->HasStealthAura() || character->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || character->IsDead())
        {
            handler->SendSysMessage(VIP_CANT_DO);
            handler->SetSentErrorMessage(true);
            return false;
        }

        handler->GetSession()->GetPlayer()->DurabilityRepairAll(false, 0, false);
        handler->PSendSysMessage(LANG_YOUR_ITEMS_REPAIRED, handler->GetNameLink(handler->GetSession()->GetPlayer()).c_str());
        return true;
    }

    static bool HandleVipResetTalentsCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* character = handler->GetSession()->GetPlayer();

        if (!handler->GetSession()->IsPremiumAccount())
        {
            handler->SendSysMessage(NOT_VIP);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!sWorld->getBoolConfig(CONFIG_VIP_RESET_TALENTS_COMMAND))
        {
            handler->SendSysMessage(VIP_COMMAND_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (character->IsInCombatActive() || character->IsInFlight() || character->GetMap()->IsBattlegroundOrArena() || character->HasStealthAura() || character->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || character->IsDead())
        {
            handler->SendSysMessage(VIP_CANT_DO);
            handler->SetSentErrorMessage(true);
            return false;
        }

        handler->GetSession()->GetPlayer()->resetTalents(true);
        handler->GetSession()->GetPlayer()->SendTalentsInfoData(false);
        handler->PSendSysMessage(LANG_RESET_TALENTS_ONLINE, handler->GetNameLink(handler->GetSession()->GetPlayer()).c_str());
        return true;
    }

    static bool HandleVipTaxiCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* character = handler->GetSession()->GetPlayer();

        if (!handler->GetSession()->IsPremiumAccount())
        {
            handler->SendSysMessage(NOT_VIP);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!sWorld->getBoolConfig(CONFIG_VIP_TAXI_COMMAND))
        {
            handler->SendSysMessage(VIP_COMMAND_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (character->IsInCombatActive() || character->IsInFlight() || character->GetMap()->IsBattlegroundOrArena() || character->HasStealthAura() || character->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || character->IsDead())
        {
            handler->SendSysMessage(VIP_CANT_DO);
            handler->SetSentErrorMessage(true);
            return false;
        }

        character->SetTaxiCheater(true);
        handler->PSendSysMessage(LANG_YOU_GIVE_TAXIS, handler->GetNameLink(character).c_str());
        if (handler->needReportToTarget(character))
            ChatHandler(character).PSendSysMessage(LANG_YOURS_TAXIS_ADDED, handler->GetNameLink().c_str());
        return true;
    }

    static bool HandleVipHomeCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* character = handler->GetSession()->GetPlayer();

        if (!handler->GetSession()->IsPremiumAccount())
        {
            handler->SendSysMessage(NOT_VIP);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!sWorld->getBoolConfig(CONFIG_VIP_HOME_COMMAND))
        {
            handler->SendSysMessage(VIP_COMMAND_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (character->IsInCombatActive() || character->IsInFlight() || character->GetMap()->IsBattlegroundOrArena() || character->HasStealthAura() || character->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || character->IsDead())
        {
            handler->SendSysMessage(VIP_CANT_DO);
            handler->SetSentErrorMessage(true);
            return false;
        }

        character->RemoveSpellCooldown(8690,true);
        character->CastSpell(character,8690,false);
        return true;
    }

    static bool HandleVipCapitalCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* character = handler->GetSession()->GetPlayer();

        if (!handler->GetSession()->IsPremiumAccount())
        {
            handler->SendSysMessage(NOT_VIP);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!sWorld->getBoolConfig(CONFIG_VIP_CAPITAL_COMMAND))
        {
            handler->SendSysMessage(VIP_COMMAND_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (character->IsInCombatActive() || character->IsInFlight() || character->GetMap()->IsBattlegroundOrArena() || character->HasStealthAura() || character->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || character->IsDead())
        {
            handler->SendSysMessage(VIP_CANT_DO);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (character->GetTeam() == HORDE)
            character->CastSpell(character,3567,true);
        else
			character->CastSpell(character,3561,true);

		return true;
    }
};

void AddSC_vip_commandscript()
{
    new vip_commandscript();
}