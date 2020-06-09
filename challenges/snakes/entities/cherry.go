package entities

import (
	"math/rand"
	"time"

	"github.com/hajimehoshi/ebiten"
	"github.com/hajimehoshi/ebiten/ebitenutil"
)

// Cherry : Object which snakes eats
type Cherry struct {
	game   *Game
	cherry ebiten.Image
	xLimit int
	yLimit int
	xPos   float64
	yPos   float64
	eaten  bool
}

// CreateCherry : Generates a Cherry
func CreateCherry(g *Game) *Cherry {
	c := Cherry{
		game:   g,
		xLimit: 30,
		yLimit: 30,
		eaten:  false,
	}
	cherry, _, _ := ebitenutil.NewImageFromFile("assets/cherry.png", ebiten.FilterDefault) //loads the cheryr img
	c.cherry = *cherry
	seed := rand.NewSource(time.Now().UnixNano())
	random := rand.New(seed)
	c.xPos = float64(random.Intn(c.xLimit) * 20) //generates a random point for cherries to appear
	c.yPos = float64(random.Intn(c.yLimit) * 20)
	return &c
}

// Update : Logical update of the snake
func (c *Cherry) Update(dotTime int) error {
	if c.eaten == false {
		return nil // Notify that a cherry has been eaten
	}
	return nil
}

// Draw the cherry
func (c *Cherry) Draw(screen *ebiten.Image, dotTime int) error {
	characterDO = &ebiten.DrawImageOptions{}
	characterDO.GeoM.Translate(c.xPos, c.yPos)
	screen.DrawImage(&c.cherry, characterDO) //every cherry is drawn here with its specific point
	return nil
}
