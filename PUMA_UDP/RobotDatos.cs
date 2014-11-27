#region Using directives

using System;

#endregion

namespace Gecko
{
    /// <summary>
    /// Estructura que contiene datos sobre un robot
    /// </summary>
    public struct RobotDatos    {
        public bool     Puma;       //Indica si el Robot es Puma o Contrario
        public int      RobNum;     //Indica el # asignado al robot
        public float    posX;       //Indica la coord X en la cancha del robot
        public float    posY;       //Indica la coord X en la cancha del robot
        public float    Angulo;     //Indica el ángulo con el que ve el robot
        public float    Velocidad;    //Velocidad        
    }

    /// <summary>
    /// almacena las caracteristicas de la cancha
    /// </summary>
    public struct Cancha
    {
        public float anchoX;        //ancho de la cancha
        public float largoY;        //largo de la cancha
    }
}
