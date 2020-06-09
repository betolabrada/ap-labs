package entities

import (
	"time"

	"github.com/hajimehoshi/ebiten"
)

// Game : Main object of the scene. Parent of everything
type Game struct {
	snake       *Snake
	snakeChan   chan int
	hud         *Hud
	cherries    []*Cherry
	numCherries int
	numEnemies  int
	enemies     []*EnemySnake
	enemiesChan []chan int
	playing     bool
	points      int
	dotTime     int
}

// NewGame : Starts a new game assigning variables
func NewGame(cherrys int, enemies int) Game {
	g := Game{
		playing:     true,
		points:      0,
		dotTime:     0,
		numCherries: cherrys,
		numEnemies:  enemies,
	}
	arrayC := make([]*Cherry, g.numCherries) //store all the cherries
	for i := 0; i < g.numCherries; i++ {
		arrayC[i] = CreateCherry(&g)
		time.Sleep(20)
	}
	arrayEnemies := make([]*EnemySnake, g.numEnemies) //store the enemies
	for i := 0; i < len(arrayEnemies); i++ {
		arrayEnemies[i] = CreateEnemySnake(&g)
		time.Sleep(20)
	}
	enemiesChan := make([]chan int, g.numEnemies)
	for i := 0; i < len(enemiesChan); i++ {
		enemiesChan[i] = make(chan int)
		arrayEnemies[i].behavior = enemiesChan[i]
		go arrayEnemies[i].Behavior()
		time.Sleep(20)
	}
	g.enemiesChan = enemiesChan //make the references for the class
	g.cherries = arrayC
	g.enemies = arrayEnemies
	g.snake = CreateSnake(&g)
	g.snakeChan = make(chan int)
	go g.snake.Behavior()
	g.hud = CreateHud(&g, cherrys)
	return g
}

// End the game
func (g *Game) End() {
	g.playing = false //booleand to keep playing
}

// Update the main process of the game
func (g *Game) Update() error {
	if g.playing {
		if g.numCherries == 0 { //when all cherries has been eating the game ends
			g.playing = false
		}
		//update the channels
		g.dotTime = (g.dotTime + 1) % 20
		if err := g.snake.Update(g.dotTime); err != nil {
			g.snakeChan <- g.dotTime
		}
		for i := 0; i < len(g.enemiesChan); i++ {
			g.enemiesChan[i] <- g.dotTime
		}
		xPos, yPos := g.snake.getHeadPos()
		for i := 0; i < len(g.cherries); i++ {
			if xPos == g.cherries[i].xPos && yPos == g.cherries[i].yPos { //if snake eats a cherry grows
				g.cherries[i].yPos = -20
				g.cherries[i].xPos = -20
				g.hud.addPoint()
				g.numCherries--
				g.snake.addPoint()
				break
			}
		}
		for j := 0; j < len(g.enemies); j++ { //if enemi snake eats cherry grows
			xPos, yPos := g.enemies[j].getHeadPos()
			for i := 0; i < len(g.cherries); i++ {
				if xPos == g.cherries[i].xPos && yPos == g.cherries[i].yPos {
					g.cherries[i].yPos = -20
					g.cherries[i].xPos = -20
					g.numCherries--
					g.enemies[j].addPoint()
					break
				}
			}
		}
	} else {
		//fmt.Println("game stopped")
	}
	for i := 0; i < g.numCherries; i++ {
		if err := g.cherries[i].Update(g.dotTime); err != nil {
			return err
		}
	}
	return nil
}

// Draw the whole interface
func (g *Game) Draw(screen *ebiten.Image) error {
	if err := g.snake.Draw(screen, g.dotTime); err != nil {
		return err
	}
	for _, enemy := range g.enemies {
		if err := enemy.Draw(screen, g.dotTime); err != nil {
			return err
		}
	}
	if err := g.hud.Draw(screen); err != nil {
		return err
	}
	for i := 0; i < len(g.cherries); i++ {
		if err := g.cherries[i].Draw(screen, g.dotTime); err != nil {
			return err
		}
	}
	if g.numCherries == 0 {
		g.hud.End2(screen)
	}
	return nil
}
