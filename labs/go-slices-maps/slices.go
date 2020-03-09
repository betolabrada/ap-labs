package main

import "golang.org/x/tour/pic"

func Pic(dx, dy int) [][]uint8 {
	var ans[][]uint8
	for i := 0; i < dy; i++ {
		ans = append(ans, make([]uint8, dx))
	}

	for y := 0; y < dy; y++ {
		for x := 0; x < dx; x++ {
			ans[y][x] = (uint8(x) + uint8(y)) / 2
		}
	}

	return ans
}

func main() {
	pic.Show(Pic)
}
