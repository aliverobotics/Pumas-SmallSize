#region Using directives

using System;

#endregion

namespace Gecko
{
    /// <summary>
    /// Contiene  la Posición(x,y), Velocidad y ángulo de la Pelota
    /// </summary>
    public struct Pelota    {
        public float posX;      //CoordX de la posición de la pelota
        public float posY;      //CoordX de la posición de la pelota
        public float Velocidad; //Velocidad de la pelota
        public float Angulo;    //Ángulo de dirección de la pelota
    }
}
