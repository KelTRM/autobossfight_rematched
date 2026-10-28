# Lua extensions

On load, the file `lua/init.lua` gets executed. This script can be used to extend the
bossfight. This file is required to be loaded, as it specifies the players/bosses.

This file specifies the API to be used within to extend the bossfight

## Table of contents
- [Entities](#entities)
 - [Adding/removing entities](#addingremoving-entities)
- [Attacks](#attacks)
 - [Attack plugin](#attackplugin)
 - [Attack handler](#attackhandler)
- [Transformations](#transformations)
- [Types](#types)
 - [Entity](#entity)
 - [AttackData](#attackdata)
  - [Notes](#notes)
# Entities

## Adding/removing entities

To add a player, you may use
```lua
bossfight:AddPlayer(
    {
        Name="foo",     -- The name to be assigned to the player.
        HP=10000,       -- The starting health of the player.
        HealMin=2500,   -- The minimum amount of healing they may give
        HealMax=7500,   -- The maximum amount of healing they may give
        Color=0xF0F0F0  -- A hex value for the color of the player's name
    }
)
```

Or for bosses, use

```lua
bossfight:AddBoss(
    {
        Name="bar"      -- The name of the boss
        HP=325000       -- The starting health of the boss
    }
)
```

The boss is missing the elements HealMin/HealMax, and Color, as they are not needed for a boss.
The heal min/max are unneeded as bosses cannot heal others,
while all bosses have the same static gray color, which cannot (for now) be changed.

# Attacks

WARNING - NOTHING IN THIS SECTION IS FINALIZED.
THIS PURELY EXISTS TO SHOW HOW IT SHOULD WORK, BUT MAY NOT STAY INTO A FINAL FORM.

## AttackPlugin

An attack may be added by a lua script by using an `AttackPlugin`.
which may be created using `bossfight:attack:NewPlugin()`, which returns a table containing the plugin.

The `AttackPlugin` contains a method to add an attack to the plugin: `AttackPlugin:AddAttack(string, table)`

The table taken in by `AddAttack` contains the data for the attack.
Below is the layout of this table

```lua
local AttackTable = {
	AttackName="example",   -- The name of the attack. This is exclusively for the display name
	RequiredEnergy=0,       -- The amount of energy the attack consumes
	EarliestRound=0,        -- The first round in which the attack can be done (nil=0)
	CanAttack=function() end, -- Tells you whether the attack can be used. use nil for default
	AttackHandler=function() end,       -- Run on attack. Determines the outcome of the attack
    id=nil                  -- The ID used to specify the attack. use nil to auto-select
}
```

While AttackHandler (for the time being) is fully functional.
attack.CanAttack is currently unimplemented

The definitions for the both functions ae as following
```lua
function AttackHandler(self, attack)
    -- self - The table for the current attack
    -- attack - A table containing the following
    --      - ["target"]    -- The entity in which the attack is targetting
    --      - ["attacker"]  -- THe entity in which is performing the attack
end

function CanAttack(self, attacker)
    -- self - The current attack
    -- attacker - The entity wishing to preform the attack
end
```

Once all your attacks have been added to the `AttackPlugin`, you must register the plugin with the bossfight.
This may be done with `bossfight.attack:RegisterPlugin(AttackPlugin)`

Below is an example of this process.
```lua
-- Create the plugin
local plugin = bossfight.attack:NewPlugin()

-- Add the attacks to the plugin
plugin:AddAttack(AttackTable)

-- Register the plugin
bossfight.attack:RegisterPlugin(plugin)

-- Attack "example" should now be visible
```

## AttackHandler

The `AttackHandler` definition should always return a table. Returning anything else will result in (crash or nothing)
on using the attack. This table may be generated using `bossfight.attack:AttackData(attack)`.
This will give you a manager for the data within this table.

While the table may be created manually, this is probably a bad idea.
The definition for the table is not guarenteed to stay the same between versions,
and a manually defined table may not be valid in future versions.

The `attack` parameter contains a table including information about the attack being preformed.
The table contains both a `from` and `target` parameter, each being entities.

# Transformations

Like with attacks, transformations are defined through plugins via
`bossfight.transformation:NewPlugin()`.

These plugins may be used to register transformations, via `plugin:AddTransformations(table)`

The table for defining a transformation are the following
```lua
local plugin=bossfight.transformation:NewPlugin()

plugin:AddTransformation({
    ["name"]="example"          -- The name of the transformation
    ["cost"]=10                 -- The cost to transform
    ["upkeep"]=10               -- The cost to continue using the transformation
    ["multiplier"]=1            -- A multiplier for attack damage. For use by the default handler
    ["on_damage"]=function()end -- Called apon user's attack. Modifies damage. Use nil for default 
    ["on_upkeep"]=function()end -- Called at the end of a round apon each player with the form.
    ["description"]="example"   -- The description of the form to be shown on 
})
```

```lua
```

The functions `on_damage` and `on_upkeep` are the implementation of the transformation.
While these can be manually implemented, it's worth noting that they each have
default implementations. The default for `on_damage` multiplies the damage by `multiplier`, while
the default for `on_upkeep` removes `upkeep` energy from the user.

If you want more complex parameters than this, then you may implemnent them yourselves,
as shown below

```lua
function on_upkeep(self, user)
    -- user: Entity (The entity using the transformation)
    return user:DrainEnergy(self.upkeep)
end

function on_damage(self, user, base)
    -- user: Entity (The entity using the transformation)
    -- base: number (the base damage being done)
    return base * self.multiplier
end
```

Also, like with attacks: The plugins must be registered.
A transformation plugin may be registered with `transformation:RegisterPlugin()`

A full example of a transformation is shown below
```lua
local plugin = bossfight.transformation:NewPlugin();

plugin:NewEntry("example1", {
    ["name"]="Example 1",
    ["cost"]=20,
    ["upkeep"]=7,
    ["multiplier"]=1.5,
    ["on_damage"]=function(self, user, base)
        print("When someone does damage with this form, You'll see this be printed")
        return base * self.multiplier;
    end,
    ["on_upkeep"]=nil   -- not implemented yet
});

plugin:NewEntry("example2", {
    ["name"]="Example 2",
    ["cost"]=15,
    ["upkeep"]=10,
    ["multiplier"]=2,
    ["on_damage"]=nil   -- on_damage can be set to nil to use default
    ["on_upkeep"]=nil
});

bossfight.transformation:RegisterPlugin(plugin);
```

# Types

All following types are contructed from tables,
which may be used to interact with the elements of the fight.

While these are all tables, it's recommended to not use these as tables directly,
but use their APIs for interacting with them.

Some, such as entities, are unable to work through traditional table definitions.
This is due to direct interaction with the internals to get/modify these

## Entity

The entity type is responsible for interacting with players and bosses.

The entity cannot be directly interacted with, and must be used by the below API
```lua
entity:GetHealth()          -- returns number of health points
entity:GetEnergy()          -- returns energy as a percentage
entity:DrainEnergy(energy)  -- drains the entity's energy
entity:Heal(target, hp)     -- heals entity by hp, returns amount healed
entity:Attack(target,hp)    -- removes hp from target's health
entity:Living()             -- returns whether the entity is alive
entity:GetEnemies()         -- gets the enemies of the entity
```

## AttackData

`AttackData` values are for AttackHandler return values.
An instance may be created using `bossfight.attack:AttackData(attack)` (attack param optional)

The AttackData type has the following data points attached to it.

 - Target (target of the given attack) entity
 - Attacker (attacker of the target) entity
 - Damage (damage dealt) number
 - Announcer (method to handle attack message) function
 - ID (leave nil) number

All of the following can be altered using `AttackData:SetX(X)`
For example

```lua
local attack = bossfight.attack:AttackData()
attack:SetDamage(100)
```

You may also find some additional announcers in bossfight.announcers, such as

 - default
 - heal

### Notes

When the attack heals the target, the Damage value is used to indicate the HP given to the target
