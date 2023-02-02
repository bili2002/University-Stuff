var dH = 15;
var dW = 28;

var lab = [];
var L = [];
var R = [];
var X;
var Y;


function plotV0()
{
    lab[X][Y] = true;
    lab[X][Y+1] = true;
    X++;
}

function plotV()
{
    lab[X+1][Y] = true;
    lab[X+1][Y+1] = true;
}

function plotH()
{
    lab[X][Y+1] = true;
    lab[X+1][Y+1] = true;
}

function plotN()
{
    X = 0;
    Y = Y+2;
}

function generateLabyrinth()
{
    // Изчиства лабиринта
    for( var x=0; x<2*dW; x++)
	{
		lab[x] = [];
        for( var y=0; y<2*dH; y++)
            lab[x][y]=false;
	}
	
    X=0;
    Y=0;
    L[0]=1;
    var E=dW;
    while (E)
    {
        L[E]=E;
        R[E]=E;
        E--;
        if (E) {plotH();}
        X+=2;
    }
    lab[X-2][Y+1]=true;
    plotN();
    plotV0();

    var H=dH;
    while( H>1 )
    {
        H--;
        var C=dW;
        while( C>1 )
        {
            C--;
            E=L[C-1];
            if( (C!=E)  && (Math.random()>0.5) )
            {
                R[E]=R[C];
                L[R[C]]=E;
                R[C]=C-1;
                L[C-1]=C;
            }
            else
            {
                plotV();
            }

            E=L[C];
            if( (C!=E)  && (Math.random()>0.5) )
            {
                R[E]=R[C];
                L[R[C]]=E;
                L[C]=C;
                R[C]=C;
                plotH();
            }

            lab[X+1][Y+1] = true;
            X = X+2;
        }

        plotN();
        plotV0();
    }


    var C=dW;
    while( C>1 )
    {
        C--;
        E=L[C-1];
        if( (C!=E)  && ((C==R[C]) || (Math.random()>0.5)) )
        {
            R[E]=R[C];
            L[R[C]]=E;
            R[C]=C-1;
            L[C-1]=C;
        }
        else
        {
            plotV();
        }

        E=L[C];
        R[E]=R[C];
        L[R[C]]=E;
        L[C]=C;
        R[C]=C;
        plotH();
        X=X+2;
    }
}

function isFree( x, y )
{
	return !isWall( x, y );
}

function isWall( x,y )
{
	return (lab[x] && lab[x][y]);
}