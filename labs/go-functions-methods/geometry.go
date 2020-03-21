// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 156.

// Package geometry defines simple types for plane geometry.
//!+point
package main

import (
	"math"
	"fmt"
	"math/rand"
	"os"
	"strconv"
	"time"
)

type Point struct{ x, y float64 }

// traditional function
func Distance(p, q Point) float64 {
	return math.Hypot(q.x-p.x, q.y-p.y)
}

// same thing, but as a method of the Point type
func (p Point) Distance(q Point) float64 {
	return math.Hypot(q.x-p.x, q.y-p.y)
}

//!-point

//!+path

// A Path is a journey connecting the points with straight lines.
type Path []Point

// Distance returns the distance traveled along the path.
// MODIFYING: sum does not consider point[i] to point[0], also adding print statements
func (path Path) Distance() float64 {
	sum := 0.0
	dist := 0.0
	n := len(path)
	for i := range path {
		if i > 0 {
			dist = path[i-1].Distance(path[i])
			sum += dist
			fmt.Printf(" %0.2f + ", dist)
		}
	}
	dist = path[n - 1].Distance(path[0])
	sum += dist
	fmt.Printf(" %0.2f = ", dist)

	return sum
}

//!-path

func (point Point) X() float64 {
	return point.x
}

func (point Point) Y() float64 {
	return point.y
}

func generateRandom(min, max float64) float64 {
	return min + rand.Float64() * (max + 1 - min)
}

func onSegment(p, q, r Point) bool {
	return q.x <= math.Max(p.x, r.x) && q.x >= math.Min(p.x, r.x) && 
	q.y <= math.Max(p.y, r.y) && q.y >= math.Min(p.y, r.y)
}

func orientation(p, q, r Point) int {
	val := (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y)
	if val == 0 {
		return 0
	}
	if val > 0 {
		return 1
	} else {
		return 2
	}
}

func doIntersect(p1, p2, q1, q2 Point) bool {
	// Orientation 0: colinear 1: clockwise 2: counterclockwise
	p1p2q1 := orientation(p1, p2, q1)
	p1p2q2 := orientation(p1, p2, q2)
	q1q2p1 := orientation(q1, q2, p1)
	q1q2p2 := orientation(q1, q2, p2)

	// General case
	if p1p2q1 != p1p2q2 && q1q2p1 != q1q2p2 { return true }

	// Special case
	if p1p2q1 == 0 && onSegment(p1, q1, p2) { return true }
	if p1p2q2 == 0 && onSegment(p1, q2, p2) { return true }
	if q1q2p1 == 0 && onSegment(q1, p1, q2) { return true }
	if q1q2p2 == 0 && onSegment(q1, p2, q2) { return true }

	return false

} 

func printResults(points Path) {
	// Printing vertices
	fmt.Println("- Figure's vertices")
	for i := range points {
		fmt.Printf("\t- ( %6.2f, %6.2f)\n", points[i].X(), points[i].Y())
	}
	fmt.Println("- Figure's perimeter: ")
	fmt.Printf("\t-")
	fmt.Printf(" %0.2f\n", points.Distance())
}

func main() {
	sides, err := strconv.Atoi(os.Args[1])

	if err != nil {
		fmt.Println(err)
	}
	pts := make(Path, sides) 

	bIntersect := false

	fmt.Printf("- Generating a [%d] sides figure\n", sides)

	// Random points, loop until we find points whose lines do not intersect with each other
	for {
		rand.Seed(time.Now().UnixNano())
		for i := 0; i < sides; i++ {
			pts[i] = Point{generateRandom(-100.0, 100.0), generateRandom(-100.0, 100.0)}
			// fmt.Printf("{ %6.2f, %6.2f }\n", pts[i].X(), pts[i].Y())
		}
	
		// Check segments
		for i := 0; i < sides; i++ {
			if doIntersect(pts[i%sides], pts[(i+1)%sides], pts[(i+2)%sides], pts[(i+3)%sides]) {
				fmt.Println("\n !! Points intersect each other, trying again...")
				fmt.Println();
				bIntersect = true
				break
			}
		}

		// Test run size 4
		// pts[0] = Point{-3, 1}
		// pts[1] = Point{2, 3}
		// pts[2] = Point{0, 0}
		// pts[3] = Point{-1.5, -2.5}

		if !bIntersect {
			printResults(pts)
			break
		} else {
			bIntersect = false
		}

		


	}


	

}
