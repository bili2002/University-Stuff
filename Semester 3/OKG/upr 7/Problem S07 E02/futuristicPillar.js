// Примерен обект-пилон

function futuristicPillar(x,z, height, material)
{
	var pillar = new THREE.Group();

	var obj = new THREE.Mesh(
		new THREE.SphereGeometry( 4, 32, 32 ),
		material
	);
	obj.position.set( 0, height, 0 );
	obj.receiveShadow = true;
	obj.castShadow = true;
	pillar.add( obj );

	var spline = new THREE.QuadraticBezierCurve( 
		new THREE.Vector3( Math.max(1+height/2,10), 0, 0 ),
		new THREE.Vector3( -3, 0, 0 ),
		new THREE.Vector3( 2, height-1, 0 ) );
	var points = [];
	for ( var i = 0; i <= 30; i ++ )
	{
		var p = spline.getPoint(i/30);
		points.push( new THREE.Vector2( p.x, p.y ) );
	}

	obj = new THREE.Mesh(
		new THREE.LatheGeometry( points, 40 ),
		material
	);

	obj.receiveShadow = true;
	obj.castShadow = true;
	pillar.add( obj );
	pillar.position.set( x, 0, z );	
	
	return pillar;
}