# Lua extensions

WARNING - THIS FEATURE IS INCOMPLETE. EVERYTHING IS A WORK IN PROGRESS AND IS SUBJECT TO CHANGE

## Entities

### Adding/removing entities

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

### Modifying existing entities

All entities when created are stored in either `bossfight.Players`, or `bossfight.Bosses` (with an obvious distinction).
These values both contain arrays storing the tables passed into AddPlayer/AddBoss, which may be modified at any time.

As an effect, you can circumvent the AddBoss/AddPlayer methods alltogether in the following manner

```lua
bossfight.Players[1] = { Name="foo", HP=10000, HealMin=2500, HealMax=7500, Color=0xF0F0F0 }
bossfight.Bosses[1] = { Name="bar", HP=325000 }
```
