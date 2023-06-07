import Swift

func countFolders(paths: [String]) -> Int {
	var folders: Set<Substring> = [];
	
	for path in paths { 
		var dot = false	
		
		for (i, char) in path.enumerated() {
			if char == "/" {
				folders.insert(path.prefix(i))
				dot = false;
			} else if char == "." {
				dot = true;
			}
		}
		
		if let temp = path.last {
			if  dot == false { 
				folders.insert(Substring(path))
			}
		}
		
	}
	
	return folders.count
}
/*
var paths = ["/", "/a", "/b", "/b.a", "/a.b/", "/a/b.b", "/a/b.b/a.b"];
print(countFolders(paths: paths));*/