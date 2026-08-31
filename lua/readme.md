# Lua extensions

WARNING - THIS FEATURE IS INCOMPLETE. EVERYTHING IS A WORK IN PROGRESS AND IS SUBJECT TO CHANGE

# Scripts

At load time, the file `lua/script.lua` is loaded. This file is the main script,
and is responsible for loading all other scripts.
Think of this file as a place to say what scripts you do and don't want to use.

## Adding scripts

The system for adding and removing scripts acts as a state machine

The state machine contains the following

 - A list of all required files for this script
 - The current type of script to add (`attack` or `entity`)

The list, containing the value of the type as of when it was added.

This may be interacted with by the following interface
```lua
local dependencies = bossfight:GetDependencyList();

dependencies:SetType("entity")
dependencies:AddDependency("entity.lua")

dependencies:SetType("attack")
dependencies:AddDependency("attack.lua")
```

Note - A dependency list may be created by any lua script,
but a script may not load one which can do something beyond its permission.
This means while you could put everything within script.lua,
it's better to keep everything separate as to keep everything organized.

On top of that, it's best to keep script.lua custom to your game.
The current iteration of the scripting system has no ability to change what's in a different script,
but that's not guarenteed to remain the same and it could end up causing damage in the future.

# Entities

## Adding/removing entities

On load, the file lua/entities.lua is loaded and executed.
this file is responsible for specifying the players and bosses which will participate in the game.

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

## Modifying existing entities

All entities when created are stored in either `bossfight.Players`, or `bossfight.Bosses` (with an obvious distinction).
These values both contain arrays storing the tables passed into AddPlayer/AddBoss, which may be modified at any time.

As an effect, you can circumvent the AddBoss/AddPlayer methods alltogether in the following manner

```lua
bossfight.Players[1] = { Name="foo", HP=10000, HealMin=2500, HealMax=7500, Color=0xF0F0F0 }
bossfight.Bosses[1] = { Name="bar", HP=325000 }
```

# Attacks

WARNING - NOTHING IN THIS SECTION IS FINALIZED.
THIS PURELY EXISTS TO SHOW HOW IT SHOULD WORK, BUT MAY NOT STAY INTO A FINAL FORM.

## AttackPlugin

An attack may be added by a lua script by using an `AttackPlugin`.
which may be created using `bossfight:CreateAttackPlugin()`, which returns a table containing the plugin.

The `AttackPlugin` contains a method to add an attack to the plugin: `AttackPlugin:AddAttack(string, table)`

The table taken in by `AddAttack` contains the data for the attack.
Below is the layout of this table

```lua
local AttackTable = {
	AttackName="example",   -- The name of the attack. This is exclusively for the display name
	RequiredEnergy=0,       -- The amount of energy the attack consumes
	EarliestRound=0,        -- The first round in which the attack can be done (nil=0)
	CanAttack=function(proposed_attack) return true end, -- Tells you whether the attack can be used. use nil for default
	AttackHandler=function(attack) return { } end,       -- Run on attack. Determines the outcome of the attack
    id=nil                  -- The ID used to specify the attack. use nil to auto-select
}
```

Most of these are pretty straight forward.
The only things worth noting are the details of `CanAttack` and `AttackHandler`,
which will be described in the section below

Once all your attacks have been added to the `AttackPlugin`, you must register the plugin with the bossfight.
This may be done with `bossfight.attack:RegisterAttackPlugin(AttackPlugin)`

Below is an example of this process.
```lua
-- Create the plugin
local plugin = bossfight.attack:CreateAttackPlugin()

-- Add the attacks to the plugin
plugin:AddAttack(AttackTable)

-- Register the plugin
bossfight.attack:RegisterAttackPlugin(plugin)

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
