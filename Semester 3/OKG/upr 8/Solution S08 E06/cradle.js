// Създава двата "конеца" и топката в края им
function cradleBall( black, crystal )
{
	var ball = new THREE.Group();
	
	// топката
	var obj = new THREE.Mesh( new THREE.IcosahedronGeometry( 6, 1 ), crystal );
	obj.position.set( 0, -40, 0 );
	ball.add( obj );
	
	// ръбчетата ѝ
	var obj = new THREE.Mesh(
		new THREE.IcosahedronGeometry( 6, 1 ),
		new THREE.MeshLambertMaterial( {color: 'white', transparent: true, opacity: 0.1, wireframe: true})
	);
	obj.position.set( 0, -40, 0 );
	ball.add( obj );
	
	var angle = 0.56;
	var len = 41;
	
	// едната връв
	obj = new THREE.Mesh( new THREE.CylinderGeometry( 1/4, 1/4, len ), black );
	obj.position.set( 0, -len*Math.cos(angle)/2, 3+len*Math.sin(angle)/2 );
	obj.rotation.x = angle;
	ball.add( obj );
	
	// другата връв
	obj = new THREE.Mesh( new THREE.CylinderGeometry( 1/4, 1/4, len ), black );
	obj.position.set( 0, -len*Math.cos(angle)/2, -3-len*Math.sin(angle)/2 );
	obj.rotation.x = -angle;
	ball.add( obj );
	
	return ball;
}

// Създава твърдата, неподвижна част на Люлката на Нютон
// Параметри са три материала
function cradleBase( black, gold, silver )
{
	var cradle = new THREE.Group();

	// основа
	var obj = new THREE.Mesh( new THREE.BoxGeometry( 100, 5, 50 ), black );
	cradle.add( obj );
	
	// четири крака и вертикални рамки
	var pos = [ 50, 25, -50, 25, -50, -25, 50, -25 ];
	for( var i=0; i<4; i++ )
	{
		// краче
		obj = new THREE.Mesh( new THREE.CylinderGeometry( 5, 5, 10, 32 ), gold );
		obj.position.set( pos[2*i], 0, pos[2*i+1] );
		cradle.add( obj );
		
		// горна заобленост на крачето
		obj = new THREE.Mesh( new THREE.SphereGeometry( 5, 32, 32 ), gold );
		obj.position.set( pos[2*i], 5, pos[2*i+1] );
		cradle.add( obj );
		
		// вертикална рамка
		obj = new THREE.Mesh( new THREE.CylinderGeometry( 2, 2, 60, 16 ), silver );
		obj.position.set( pos[2*i], 30, pos[2*i+1] );
		cradle.add( obj );
		
		// заобленост на горния край на рамката
		obj = new THREE.Mesh( new THREE.SphereGeometry( 2, 16, 16 ), silver );
		obj.position.set( pos[2*i], 60, pos[2*i+1] );
		cradle.add( obj );
	}
			
	// дълги хоризонтални рамки
	obj = new THREE.Mesh( new THREE.CylinderGeometry( 1, 1, 100, 16 ), silver );
	obj.rotation.z = Math.PI/2;
	obj.position.set( 0, 60, 25 );
	cradle.add( obj );
	
	obj = new THREE.Mesh( new THREE.CylinderGeometry( 1, 1, 100, 16 ),silver );
	obj.rotation.z = Math.PI/2;
	obj.position.set( 0, 60, -25 );
	cradle.add( obj );
			
	// къси хоризонтални рамки
	obj = new THREE.Mesh( new THREE.CylinderGeometry( 1, 1, 50, 16 ), silver );
	obj.rotation.x = Math.PI/2;
	obj.position.set( 50, 60, 0 );
	cradle.add( obj );
	
	obj = new THREE.Mesh( new THREE.CylinderGeometry( 1, 1, 50, 16 ),silver );
	obj.rotation.x = Math.PI/2;
	obj.position.set( -50, 60, 0 );
	cradle.add( obj );
	
	return cradle;
}