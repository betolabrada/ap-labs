package main

import (
	"Snakez/entities"
	"fmt"
	_ "image/png"
	"log"
	"os"
	"strconv"

	"github.com/hajimehoshi/ebiten"
)

var gm entities.Game
var cherryN int
var enemies int

func init() {
	if len(os.Args) < 2 { //check the arguments are correct
		fmt.Println("Error. Cherry number missing")
		os.Exit(3)
	}

	if len(os.Args) < 3 {
		fmt.Println("Error. Enemies number missing")
		os.Exit(3)
	}

	if len(os.Args) > 3 {
		fmt.Println("Error. Too many arguments")
		os.Exit(3)
	}
	var err error
	cherryN, err = strconv.Atoi(os.Args[1])
	if err != nil {
		fmt.Println("Error. Only numeric values for cherrys")
		os.Exit(3)
	}

	enemies, err = strconv.Atoi(os.Args[2])
	if err != nil {
		fmt.Println("Error. Only numeric values for enemies")
		os.Exit(3)
	}
	gm = entities.NewGame(cherryN, enemies)

}

// Game interface of ebiten
type Game struct {
}

// Update the main thread of the game
func (g *Game) Update(screen *ebiten.Image) error {
	if err := gm.Update(); err != nil {
		return err
	}
	return nil
}

// Draw renders the image windows every tick
func (g *Game) Draw(screen *ebiten.Image) {
	if err := gm.Draw(screen); err != nil {
		fmt.Println(err)
	}
}

// Layout : Function which executes when it needs to reajust
func (g *Game) Layout(outsideWidth, outsideHeight int) (screenWidth, screenHeight int) {
	return 600, 600
}

func main() {
	ebiten.SetWindowSize(600, 600)
	ebiten.SetWindowTitle("Gosnakes")

	if err := ebiten.RunGame(&Game{}); err != nil {
		log.Fatal(err)
	}
}
