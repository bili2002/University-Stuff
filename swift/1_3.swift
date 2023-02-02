import Swift

struct Maze {
	var n: Int = 0, m: Int = 0
	var begin: (Int, Int) = (0, 0)
	var end: (Int, Int) = (0, 0)
	
	var field = [[Bool]]() //true if you can pass through the field, false otherwise
	var used = [[Bool]]()
	
	init() { 
	}
}

func canPass(maze2: inout Maze, x: Int, y: Int) -> Bool {
	if x < 0 || x >= maze2.n || y < 0 || y >= maze2.m {
		return false
	} 
	if !maze2.field[x][y] || maze2.used[x][y] {
		return false
	}
		
	return true
}

func recursion(maze2: inout Maze, x: Int, y: Int) -> Int {
	if (x, y) == maze2.end {
		/*for (_, row) in maze2.used.enumerated() {
			for (_, cell) in row.enumerated() {
				print(cell, terminator: " ")
			}
			print()
		}
		print()*/
		return 1;
	}
	
	var count = 0;
	maze2.used[x][y] = true;
	
	if canPass(maze2: &maze2, x: x-1, y: y) {
		count += recursion(maze2: &maze2, x: x-1, y: y)
	}
	if canPass(maze2: &maze2, x: x+1, y: y) {
		count += recursion(maze2: &maze2, x: x+1, y: y)
	}	
	if canPass(maze2: &maze2, x: x, y: y-1) {
		count += recursion(maze2: &maze2, x: x, y: y-1)
	}
	if canPass(maze2: &maze2, x: x, y: y+1) {
		count += recursion(maze2: &maze2, x: x, y: y+1)
	}
	
	maze2.used[x][y] = false;
	return count;
}
	
func findPaths(maze: [[String]]) -> Int {
	var maze2 : Maze = Maze()
	
	maze2.n = maze.count;
	maze2.m = maze[0].count;
	
	for (i, row) in maze.enumerated() {
		maze2.field.append([])
		maze2.used.append([])
		for (j, cell) in row.enumerated() {
			switch cell {
				case "^" : 
					maze2.begin = (i, j)
					maze2.field[i].append(true);
					maze2.used[i].append(false);
				case "*" : 
					maze2.end = (i, j)
					maze2.field[i].append(true);
					maze2.used[i].append(false);
				case "0" : 
					maze2.field[i].append(true);
					maze2.used[i].append(false);
				default : 
					maze2.field[i].append(false);
					maze2.used[i].append(false);
			}
			//print (maze2.field[i][j], terminator: " ");
		}
		//print();
	}
	
	let ans = recursion(maze2: &maze2, x: maze2.begin.0, y: maze2.begin.1)
	
	return ans;
}

/*
var testMaze = [
	["^", "0", "0", "0", "0", "0", "0", "0"],
	["0", "1", "1", "1", "1", "1", "0", "0"],
	["0", "0", "0", "0", "0", "1", "#", "0"],
	["0", "1", "1", "1", "0", "1", "1", "0"],
	["0", "1", "0", "1", "0", "0", "1", "0"],
	["0", "0", "0", "1", "0", "0", "0", "*"]]
	
print(findPaths(tempMaze: testMaze))
*/