# Units-United
A C++ turn-based strategy game made with SDL2.

Requires SDL2, SDL2_ttf, and SDL2_net libraries to build.

# Unit List
- [The Duelist](#the-duelist)
- [The Village Idiot](#the-village-idiot)
- [The Caveman](#the-caveman)
- [The Medic](#the-medic)
- [The Fighter](#the-fighter)
- [The Raid Boss](#the-raid-boss)
- [The Blademaster](#the-blademaster)
- [The Tank](#the-tank)

25+ more Units coming soon!

## The Duelist

110 **HP**,

6-10 **attack**

60 **speed**

**AGNI KAI (ACTIVE)**

The Duelist picks a foe and the two strike each other 3 times.

**COUNTERSTRIKE (PASSIVE)**

The Duelist has a 33% chance to counterattack, completely negating an incoming attack and striking back.

## The Village Idiot:

120 **HP**

5-9 **Attack**

30 **Speed**

**SIMPLY FALL OVER (ACTIVE)**

The Village Idiot says something so unbelievably stupid that a target simply falls over, taking 20 damage and getting stunned. If the target is already stunned, deal 40 damage instead of 20.

**OOPSIE DOOPSIE (PASSIVE)**

The Village Idiot's basic attacks have a 33% chance to stun the target.

**BRAIN DEAD (PASSIVE)**

The Village Idiot is so unbelievably stupid that he cannot be stunned.

## The Caveman

170 **HP**

9-13 **Attack**

10 **Speed**

**GIGA CLUB (ACTIVE)**

The Caveman smacks an enemy so hard they move to the bottom of the turn list, dealing 10 damage.

**EVOLUTION (PASSIVE)**

The Caveman gains 4 HP and 3 damage after landing a successful basic attack.

**CAVEMAN BRAIN (PASSIVE)**

The Caveman has a 50% chance of missing basic attacks.

## The Medic

110 **HP**

1-4 **Attack** or 6-8 **Heal**

15 **Speed**

**HEALTH PACK (ACTIVE)**

The Medic gains a ramping heal buff over 3 turns, healing 5 health initially, 10 the next round, and 15 on the third round.
(Not stackable)

**ANTIBIOTICS (PASSIVE)**

The Medic heals himself for 5 HP every turn.

## The Fighter

135 **HP**

5-8 **Attack**

75 **Speed**

**HAYMAKER (ACTIVE)**

The Fighter grants himself +3 damage for every enemy currently alive on the opposing team. This buff will wear off after your next use of ONE TWO COMBO.

**ONE TWO COMBO (PASSIVE)**

The Fighter can attack two people at once.

**STAND YOUR GROUND (PASSIVE)**

Once attacked, The Fighter will take 3 less damage from all subsequent damage this round.

## The Raid Boss

200 **HP**

15-20 **Attack**

1 **Speed**

**EARTHSHATTER (ACTIVE)**

The Raid Boss stuns two targets, doing 5 damage to each target. One turn Cooldown.

**RETRIBUTION (PASSIVE)** 

The Raid Boss returns 3 damage per attack.

**OVERWHELMED (PASSIVE)**

The Raid Boss increases the damage taken from all sources by 1 each time he takes damage.

**GIANT’S SPEED (PASSIVE)**

The Raid Boss has a 50% chance of missing basic attacks.

## The Blademaster

120 **HP**

9-13 **Attack**

85 **Speed**

**GUILLOTINE (ACTIVE)**

The Blademaster strikes a target for 20 damage, if it kills the target, he can strike again.

**STORM OF BLADES (PASSIVE)**

The Blademaster has a 25% chance to hit 3 times in a row.

**DISABLING BLOW (PASSIVE)**

If the Blademaster lands a successful attack, it decreases the enemy's dodge chance by 15% for one turn.

## The Tank

200 **HP**

6-10 **attack**

45 **speed**

**BIG IROH BLOCK (ACTIVE)**

For one turn, direct all damage an ally receives to The Tank. One turn cooldown.

**FORTRESS (PASSIVE)**

If The Tank takes an instance of damage under 20 damage, it will do 2 less.

**GIANTSLAYER (PASSIVE)**

If The Tank takes an instance of damage over 20 damage, it will deal +12 more.
