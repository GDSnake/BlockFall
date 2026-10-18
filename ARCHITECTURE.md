# BlockFall Architecture

The project is a small SDL3 game organized around a top-level game loop, one active scene, and a set of shared services.

## Runtime flow

```mermaid
flowchart TD
    Main[main.cpp\napplication entry point] --> Config[Config\nload config.json]
    Config --> Game[Game\ninitialise and run]

    Game --> SDL[SDL3 / SDL_ttf\ninitialisation]
    Game --> Services[Shared services]
    Services --> Renderer[Renderer\nwindow, draw calls, text textures]
    Services --> Sprites[SpriteManager\nblock sprites]
    Services --> Fonts[FontsManager\nfont handles]
    Services --> Audio[AudioManager\nmusic and sound]
    Game --> Scene[GameScene\nactive scene]

    Scene --> Input[InputManager\nSDL events, key states, DAS/ARR]
    Scene --> Logic[Gameplay state machine]
    Scene --> Field[GameField\nscore, level, speed, ruleset]
    Scene --> Render["render()"]
    Render --> Renderer

    Game --> Loop[Per-frame loop]
    Loop --> Events[Poll SDL events]
    Events --> Scene
    Loop --> Update[Scene update delta time]
    Update --> Logic
    Logic --> Render
    Loop --> Quit{shouldQuit?}
    Quit -->|no| Loop
    Quit -->|yes| Shutdown["Game::shutdown()\nrelease services and SDL"]
```

## Gameplay data flow

```mermaid
flowchart LR
    Rules[config.json\nGameRuleset] --> Field[GameField]
    Field --> Board[Board\n10 x 20 shared_ptr<Block> cells]

    Scene[GameScene] --> Current[Current PieceData\nPiece + board position]
    Scene --> Preview[Next Piece]
    Scene --> Input[InputManager]
    Input --> Move[Move / rotate / soft drop]
    Move --> Collision{Board collision\nvalidation}
    Current --> Collision
    Board --> Collision

    Collision -->|valid| Current
    Collision -->|piece reaches bottom| Commit[Save piece to Board]
    Commit --> Clear[Clear completed lines]
    Clear --> Score[Update score, lines, level]
    Score --> Field
    Score --> Spawn[Spawn next piece]
    Spawn --> Current

    Board --> Draw[Renderer draws board]
    Current --> Draw
    Preview --> Draw
    Field --> Text[Renderer draws score, level, lines]
    Text --> Draw

    Sprites[SpriteManager] --> Piece[Piece geometry + block sprite]
    Piece --> Current
    Fonts[FontsManager] --> Text
```

## Main ownership boundaries

- `main()` loads configuration, then delegates lifecycle management to `Game`.
- `Game` owns the active `GameScene` and controls initialisation, frame timing, event polling, and shutdown.
- `GameScene` owns the gameplay state machine, active/preview pieces, and `InputManager`.
- `GameField` owns ruleset-derived counters and a shared `Board`.
- `Board` stores shared block sprite references in its cells; `Piece` stores shape geometry and its block sprite.
- `Renderer`, `SpriteManager`, `FontsManager`, and `AudioManager` are shared singleton services used by the scene and lifecycle code.

## Source map

| Area | Important files | Responsibility |
| --- | --- | --- |
| Entry point | `source/main.cpp` | Load configuration and start `Game` |
| Lifecycle | `source/Game/Game.*` | SDL setup, frame loop, shutdown |
| Scene | `source/Game/Scenes/*` | Scene contract and gameplay orchestration |
| Gameplay | `source/Game/GameField.h`, `source/Game/Board.h` | Ruleset state, board collision and line clearing |
| Entities | `source/Entities/*` | Pieces, blocks, and piece data |
| Input | `source/Input/*` | SDL input translation and repeat handling |
| Rendering | `source/Core/Renderer.*` | Window, drawing, and text |
| Assets | `source/AssetManagers/*` | Sprite, font, and audio loading |
| Configuration | `source/Config/*`, `config.json` | Runtime settings and rulesets |
