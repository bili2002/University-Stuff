protocol Fillable {
        var color: String { set, get }
}

protocol VisualComponent: Fillable {

        var boundingBox: Rect { get }
        var parent: VisualComponent? { get }
        func draw()
}


protocol VisualGroup: VisualComponent {
    var numChildren: Int { get }
    var children: [VisualComponent] { get }
    func add(child: VisualComponent)
    func remove(child: VisualComponent)
    func removeChild(at: Int)
}

struct Point {
        var x: Double
        var y: Double
    }
    
struct Rect {
    //top-left
    var top:Point
    var width: Double
    var height: Double
    
    init(x: Double, y: Double, width: Double, height: Double) {
        top = Point(x: x, y: y)
        self.width = width
        self.height = height
    }
}

class Object: VisualComponent {
    var color: String
    var boundingBox: Rect
    var parent:  VisualComponent?

    func == (other1: Object, other2: Object) -> Bool {
        if(other1.color == other2.color && other1.boundingBox == other2.boundingBox) {
            return true
        }
        return false
    }

    func draw() {
    }
}

class Triangle {

    init(a: Point,  b: Point, c: Point, color:String) {
        self.color = color
        let minX = min(a.x, b.x, c.x)
        let maxX = max(a.x, b.x, c.x)
        let minY = min(a.y, b.y, c.y)
        let minX = max(a.y, b.y, c.y)

        self.boundingBox = Rect(x: minX, y: maxY, width: maxX-minX, height: maxY-minY)
    }

}
extension Triangle: Object {}


class Rectangle {

    init(x: Int, y: Int, width: Int, height: Int, color: String) {
        self.color = color
        self.boundingBox = Rect(x: x, y: y, width: width, height: height)
    }
 
}
extension Rectangle: Object {}

class Circle {

    init(x: Int, y:Int, r: Double, color: String) {
        self.color = color
        self.boundingBox = Rect(x: x - r, y: y + r, width: 2*r, height: 2*r)
    }
   
}
extension Circle: Object{}

class Path{

    init(points: [Point], color: String) {
        self.color = color

        let lowestX = points[0].x
        let highestX = points[0].x
        let lowestY = points[0].y
        let highestY = points[0].y

        for point in points {
            if(point.x < lowestX) {
                lowestX = point.x
            }
            if(point.y > highestY) {
                highestY = point.y
            }
            if(point.x > highestX) {
                highestX = point.x
            }
            if(point.y < lowestY) {
                lowestY = point.y
            }
        }
        self.boundingBox = Rect(x: lowestX, y: highestY, width: highestX- lowestX, height: highestY - lowestY)
    }

}
extension Path: Object{}

protocol StackProtocol: VisualGroup {
    func update();
}

class Stack {
    var numChildren: Int
    var color: String
    var boundingBox: Rect

    var children: [VisualComponent]

    init() {
        self.numChildren = 0
        self.color = ""
        self.children = []
    }

    func add(child: VisualComponent) {
        self.children.append(child)
        self.numChildren += 1
        update()
    }
    
    func remove(child: VisualComponent) {
        var i = 0
        for chilt in self.children {
            if(chilt == child) {
                self.children.remove(at: i)
                break
            }
            i -= 1
        }
        update()
    }
    
    func removeChild(at: Int) {
        self.children.remove(at: at)
        self.numChildren -= 1
        update()
    }
    
    var parent: VisualComponent?

    func draw() {}
}
extension Stack: StackProtocol{}

class HStack: Stack {
    update() {
        var biggestHeight = self.children[0]
        var width = 0;

        for child from self.children {
            if(biggestHeight < child.boundingBox.height) {
                biggestHeight = child.boundingBox.height
            }
            width += child.boundingBox.width
        }
        self.boundingBox = Rect(x: self.children[0].boundingBox.top.x, 
                                y: self.children[0].boundingBox.top.y, 
                                width: width, height: biggestHeight)
    }
}

class VStack: Stack {
    update() {
        var biggestWidth = self.children[0]
        var height = 0;

        for child from self.children {
            if(biggestWidth < child.boundingBox.width) {
                biggestWidth = child.boundingBox.width
            }
            height += child.boundingBox.height
        }
        self.boundingBox = Rect(x: self.children[0].boundingBox.top.x, 
                                y: self.children[0].boundingBox.top.y, 
                                width: biggestWidth ,height: height)
    }
}

class ZStack: Stack {
    update() {
       
    }
}