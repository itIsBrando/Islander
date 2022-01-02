# **To-Do List**

## Features:
- [ ] Add entities (crafting benches)
- [ ] Add iron ore
- [ ] Add iron pick
- [ ] Add benchless crafting recipes
- [x] Add crafting
- [x] Add wood
- [x] Add rocks
- [x] Add trees
- [x] Add item structures
- [x] Add hotbar
- [x] Add tile data
- [x] Add item data
- [x] Add 8x8 chunking areas
- [x] Create background movement
- [x] Add main file and game loop
- [x] Add all sprites

## Polish:
- [ ] Add player's cursor to screen shake movement
- [ ] Add more island generation
- [ ] While holding **`B`**, allow `up` and `down` to quickly cycle hotbar slot
- [ ] **Redo `cnk_write_active` to avoid unnecessary redraws**
- [ ] Cap item quantity at `99` in order to prevent graphical bugs
- [x] Add inventory quantities
- [x] Particle effects when mining items


## Bug Fixes:
- [ ] **Tiles are not persistant**
- [ ] Obstacles can spawn on top of the player
- [x] Random graphical glitches that seem to only occur after `shake()` is called frequently.
    - note that this does not seem to be a graphical issue, rather it is caused by the loading of the chunks itself
    - *FIXED!* Bug happened when player moved in an opposing direction when x=63 or y=63. Patch now prevents player from sitting at (or multiples of) those coordinates
- [x] ***PLAYER ITEM REMOVAL CAN CRASH GAME***
- [x] Graphical bug occurs when item is removed from inventory and is not the last item in the hotbar