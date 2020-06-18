-- <talkaction words="!comprar;/comprar" event="script" value="comprar.lua"/>

local config = {
    protectZone = "sim", -- Precisa estar em PZ para usar o comando?

    skill = {
        item = 9971, -- Item removido
        quantidade = 10, -- Quantidade
        quantidadeskill = 10, -- Skill adicionada
        limiteskill = 350 -- Limite
    },

    magicLevel = {
        item = 9971, -- Item removido
        quantidade = 15, -- Quantidade
        quantidademl = 5, -- Magic level adicionado
        limitedeml = 200 -- Limite
    },

    skillID = { -- ID das skills
        ["club"] = SKILL_CLUB,
        ["sword"]= SKILL_SWORD,
        ["axe"] = SKILL_AXE,
        ["distance"] = SKILL_DISTANCE,
        ["shielding"] = SKILL_SHIELD
    },

    vocationSkill = { -- Vocações que pode comprar x skill
        ["club"] = {4, 8},
        ["sword"] = {4, 8},
        ["axe"] = {4, 8},
        ["distance"] = {3, 7},
        ["shielding"] = {4, 8}
    },

    vocationMagicLevel = {1, 2, 5, 6}, -- Vocações que pode comprar magic level

    delay = {
        skill = {
        storage = 45,
        duration = 5 -- Tempo para comprar skill novamente
    },

        magicLevel = {
            storage = 97,
            duration = 5 -- Tempo para comprar magic level novamente
        }
    }
}

function onSay(cid, words, param, channel)
    if config.protectZone == "sim" and not getTilePzInfo(getCreaturePosition(cid)) then
        return doPlayerSendCancel(cid, "Você precisa estar em protection zone para comprar.")
    end

    if param == "magiclevel" then
        if getPlayerMagLevel(cid) < config.magicLevel.limitedeml then
            if isInArray(config.vocationMagicLevel, getPlayerVocation(cid)) then
                if doPlayerRemoveItem(cid, config.magicLevel.item, config.magicLevel.quantidade) then
                    if (os.time() - getPlayerStorageValue(cid, config.delay.magicLevel.storage)) >= config.delay.magicLevel.duration then
                        setPlayerStorageValue(cid, config.delay.magicLevel.storage, os.time())
                        doRemoveCreature(cid, true)
                        local playerId = getPlayerGUID(cid)
                        db.executeQuery("UPDATE `players` SET `maglevel` = `maglevel` + " .. config.magicLevel.quantidademl .. " WHERE `id` = " .. playerId)
                    else
                        doPlayerSendCancel(cid, "Espere " .. config.delay.magicLevel.duration .. " segundos para comprar novamente.")
                    end
                else
                    doPlayerSendCancel(cid, "Você não tem o item requerido.")
                end
            else
                doPlayerSendCancel(cid, "Voce não pode comprar magic level.")
            end
        else
            doPlayerSendCancel(cid, "Você chegou no limite de magic level.")
        end
    elseif config.skillID[string.lower(param)] then
        if getPlayerSkill(cid, config.skillID[string.lower(param)]) < config.skill.limiteskill then
            if isInArray(config.vocationSkill[string.lower(param)], getPlayerVocation(cid)) then
                if doPlayerRemoveItem(cid, config.skill.item, config.skill.quantidade) then
                    if (os.time() - getPlayerStorageValue(cid, config.delay.skill.storage)) >= config.delay.skill.duration then
                        setPlayerStorageValue(cid, config.delay.skill.storage, os.time())
                        doRemoveCreature(cid, true)
                        local playerId = getPlayerGUID(cid)
                        db.executeQuery("UPDATE `player_skills` SET `value` = `value` + " .. config.skill.quantidadeskill .. " WHERE `player_id` = " .. playerId .. " and `skillid` = " .. config.skillID[string.lower(param)])
                    else
                        doPlayerSendCancel(cid, "Espere " .. config.delay.skill.duration .. " segundos para comprar novamente.")
                    end
                else
                    doPlayerSendCancel(cid, "Você não tem o item requerido.")
                end
            else
                doPlayerSendCancel(cid, "Voce não pode comprar este skill.")
            end
        else
            doPlayerSendCancel(cid, "Você chegou no limite de skill.")
        end
    else
        doPlayerSendTextMessage(cid, MESSAGE_INFO_DESCR, "Possíveis parâmetros: club, sword, axe, distance, shielding, magiclevel.")
    end

    return true
end
