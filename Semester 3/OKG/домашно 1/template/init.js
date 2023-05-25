console.warn( `
 _      ___   _   ___   ___   _                
| |\\/| / / \\ | | | | \\ / / \\ | |\\/|            
|_|  | \\_\\_/ |_| |_|_/ \\_\\_/ |_|  |   

            2022-2023

`
);



// WebGL + включване на сенки

var renderer = new THREE.WebGLRenderer( {antialias:true} );
	renderer.setAnimationLoop( animate );
	renderer.shadowMap.enabled = true;
	renderer.shadowMap.type = THREE.VSMShadowMap;

	document.body.appendChild( renderer.domElement );
	document.body.style.margin = 0;
	document.body.style.overflow = 'hidden';

const MAX_ANISOTROPY = renderer.capabilities.getMaxAnisotropy();


// сцена

var scene = new THREE.Scene();
	scene.background = new THREE.Color(0.05,0.055,0.1);


// камера и гледна точка

var camera = new THREE.PerspectiveCamera( 60, 1, 0.01, 200 );
	camera.position.set( 30, 60, 60 );
	camera.lookAt( scene.position );


// основна светлина

var light = new THREE.SpotLight( 'white', 0.6 );
	light.position.set( 20, 80, 30 );
	light.target = scene;
	light.angle = Math.PI/20;
	light.penumbra = 1;
	light.castShadow = true;
	light.shadow.mapSize.width = 1024/2; 
	light.shadow.mapSize.height = 1024/2; 
	light.shadow.camera.near = 50; 
	light.shadow.camera.far = 200; 
	light.shadow.camera.left = -50; 
	light.shadow.camera.right = 50; 
	light.shadow.camera.top = -50; 
	light.shadow.camera.bottom = 50; 
	light.shadow.bias = -0.0001; 
	light.shadow.radius = 3;

	scene.add( light );


var contraLight = new THREE.SpotLight( 'white', 0.2 );
	contraLight.position.set( -10, 80, -40 );
	contraLight.target = scene;
	contraLight.angle = Math.PI/20;
	contraLight.penumbra = 1;

	scene.add( contraLight );


// допълнителна светлина

var topLight = new THREE.SpotLight( 'white', 0.2 );
	topLight.position.set( 0, 50, 0 );
	topLight.target = scene;
	topLight.angle = Math.PI/20/1;
	topLight.penumbra = 1;

	scene.add( topLight );


// обща светлина

var ambLight = new THREE.AmbientLight( 'white', 0.2 );

	scene.add( ambLight );


// помощна координатна система

var oxyz = new THREE.AxesHelper( 16 );
//	scene.add( oxyz );
	
	
// интерактивно въртене

var controls = new THREE.OrbitControls( camera, renderer.domElement );
	controls.maxPolarAngle = Math.PI * 0.45;
	controls.minDistance = 2;
	controls.maxDistance = 30;
	controls.enableDamping = true;
	controls.dampingFactor = 0.05;
	controls.rotateSpeed = 0.3;
	controls.panSpeed = 0.7;
	controls.screenSpacePanning = false;
	controls.target.set( 0, 0, 0 );
	controls.update();

	
// промяна на размера на прозореца, вкл. и завъртане на смартфон
			
window.addEventListener( 'resize', onWindowResize, false );
onWindowResize();

function onWindowResize( event )
{
	camera.aspect = window.innerWidth / window.innerHeight;
	camera.updateProjectionMatrix();

	renderer.setSize( window.innerWidth, window.innerHeight, true );
}			


// главен анимационен цикъл

function animate( time )
{
	controls.update( time );
	renderer.render( scene, camera );
}


// картинка на квадратчета

var tiles = 'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgAQMAAABJtOi3AAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAJcEhZcwAAEnQAABJ0Ad5mH3gAAAAGUExURcPDw/////joICQAAAASSURBVAjXY2AAgfr///8PTgIAnnZsBQ0uHE0AAAAASUVORK5CYII=';


// земя 20х20 метра

function ground() 
{
	var geometry = new THREE.PlaneGeometry( 30, 30 );

	var texture = new THREE.TextureLoader().load( tiles );
		texture.wrapS = THREE.RepeatWrapping;
		texture.wrapT = THREE.RepeatWrapping;
		texture.repeat.set( 30, 30 );
		texture.anisotropy = MAX_ANISOTROPY;

	var material = new THREE.MeshStandardMaterial( {
		color: 'forestgreen',
		roughness: 1,
		metalness: 0,
		map: texture,
		polygonOffset: true,
		polygonOffsetFactor: 4,
		polygonOffsetUnits: 4,
	} );
	
	var mesh = new THREE.Mesh( geometry, material );
		mesh.position.y = -0.02;
		mesh.receiveShadow = true;
		mesh.rotation.x = -Math.PI/2;

	scene.add( mesh );
}

ground();


// случайно цяло число
function random( a, b )
{
	var rnd = Math.abs( THREE.Math.seededRandom( ) );
	return Math.floor( a + (b-a+1)*rnd );
}


// под на къщата (използва FN)
function floor( x, y, sx, sy ) 
{
	if( sx%2 ) x += 0.5;
	if( sy%2 ) y += 0.5;
	
	var texture = new THREE.TextureLoader().load( tiles );
		texture.wrapS = THREE.RepeatWrapping;
		texture.wrapT = THREE.RepeatWrapping;
		texture.repeat.set( 5*sx, 5*sy );
		texture.anisotropy = MAX_ANISOTROPY;

	var material = new THREE.MeshStandardMaterial( {
		color: 'gainsboro',
		roughness: 0.6,
		metalness: 0.1,
		map: texture,
		bumpMap: texture,
		bumpScale: -0.02,
	} );

	var material2 = new THREE.MeshBasicMaterial( {
		color: 'black',
		polygonOffset: true,
		polygonOffsetFactor: 2,
		polygonOffsetUnits: 2,
	} );

	
	var geometry = new THREE.PlaneGeometry( sx, sy );

	var mesh = new THREE.Mesh( geometry, material );
		mesh.position.set( x, -0.01, y );
		mesh.receiveShadow = true;
		mesh.rotation.x = -Math.PI/2;

	scene.add( mesh );
	
	
	var geometry = new THREE.PlaneGeometry( sx+0.4, sy+0.4 );

	var mesh = new THREE.Mesh( geometry, material2 );
		mesh.position.set( x, -0.02, y );
		mesh.rotation.x = -Math.PI/2;

	scene.add( mesh );
}


// фиксиране на генератора на случайни числа
if( FN == '9MI0899999' )
	THREE.Math.seededRandom( 41027 );
else
{	
	var seed = 0;

	for( var i=0; i<FN.length; i++ )
		seed = (seed<<1)+Math.round(i*Math.cos(i+FN.charCodeAt(i)))*FN.charCodeAt(i);
	
	THREE.Math.seededRandom( seed );
}


// генериране на пода на къщата

floor( random(-2,2), random(-2,2), random(10,14), random(6,8) );
floor( random(-2,2), random(-2,2), random(6,8), random(10,14) );


