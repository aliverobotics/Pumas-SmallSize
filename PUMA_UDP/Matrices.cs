#region Using directives
using System;
using System.IO;
using System.Windows.Forms;
using Gecko;
#endregion

namespace Matrices
{
    /// <summary>
    /// Contiene lo necesario para trabajar con matrices.
    /// </summary>
    public class Matriz
    {
        /// <summary>
        /// contiene la matriz
        /// </summary>
        public double[,] Matrix;

        public int rows;
        public int cols;

        /// <summary>
        /// Constructor:
        /// Crea una matriz del tamano especificado.
        /// </summary>
        /// <param name="filas">Numero de filas que tendra la matriz</param>
        /// <param name="columnas">Numero de columnas que tendra la matriz</param>
        public Matriz(int filas,int columnas)
        {
            rows = filas;
            cols = columnas;
            Matrix=new double[rows,cols];
            for (int i = 0; i < rows; i++)//inicializar la matriz con ceros
                for (int j = 0; j < cols; j++)
                    Matrix[i, j] = 0;
        }

        /// <summary>
        /// Crea una matriz leyendo los datos de un archivo de texto, separado
        /// por coma o por espacios, del tamano definido en el constructor.
        /// </summary>
        /// <param name="File">Path al archivo donde se encuentran los datos de la Matriz</param>
        public bool LoadFromFile(string Archivo)
        {
            if (!File.Exists(Archivo))
            {//el archivo no existe
                return false;
            }
            using (StreamReader sr = File.OpenText(Archivo))
            {
                String input;
                String[] Token;
                int i, j = 0,k=0;
                
                while ((input = sr.ReadLine()) != null && k<=this.rows)
                {//se lee una linea entera, hasta que se completen las requeridas
                    Token=input.Split(new char[] { ' ', '\r', '\n', '\0' });
                    #region TokensOK
                    for (i = 0; i < Token.Length; i++)
                    {//Contar cuantos NO nulos hay
                        if (Token[i] != "")
                            j++;
                    }
                    String[] tmp = new String[j];
                    j = 0;
                    for (i = 0; i < Token.Length; i++)
                    {//Guardar sólo los NO nulos
                        if (Token[i] != "")
                            tmp[j++] = Token[i];
                    }
                    Token = tmp;
                    #endregion
                    for(i=0;i<this.cols;i++)
                    {
                        try
                        {
                            this.Matrix[0, i]=float.Parse(Token[i]);
                        }
                        catch (System.FormatException)
                        {
                            //archivo con datos no numericos
                            sr.Close();
                            return false;
                        }
                        catch (Exception)
                        {
                            //algun error no tomado en cuenta
                            sr.Close();
                            return false;
                        }
                    }
                    k++;
                }
                //EOF
                sr.Close();
                return true;
            }
        }

        /// <summary>
        /// Establece el contenido de la matriz
        /// </summary>
        /// <param name="Valores">La matriz con los datos</param>
        /// <returns>TRUE, si es correcta la asignacion. FALSE, si el tamano es incorrecto</returns>
        public bool SetData(double[,] Valores)
        {
            if (Valores.Length == Matrix.Length)
            {
                Matrix = Valores;
                return true;
            }
            else
                return false;
        }

        /// <summary>
        /// Multiplica la matriz de la clase actual, por la especificada
        /// en el el parametro PorMatriz, antes verifica si son conformables
        /// </summary>
        /// <param name="PorMatriz">Matriz por la que se pos.multiplicara</param>
        /// <returns>Si es conformable, la matriz resultante, de los contrario NULL</returns>
        public Matriz Multiplicar(Matriz PorMatriz){
            if (this.cols == PorMatriz.rows){//son conformables
                Matriz MRes= new Matriz(rows,PorMatriz.cols);
                for(int i=0;i<rows;i++)
                    for(int j=0;j<PorMatriz.cols;j++)
                        for(int k=0;k<cols;k++)
                            MRes.Matrix[i,j]+=this.Matrix[i,k]*PorMatriz.Matrix[k,j];
                MRes.rows = this.rows;
                MRes.cols = PorMatriz.cols;
                return MRes;
            }
            else
                return null;//no es posible realizar la multiplicacion, no son conformables
        }
        /// <summary>
        /// Multiplica todos los elementos de la matriz por un escalar
        /// </summary>
        public void MultiplicarEscalar(double Escalar)
        {
            for (int i = 0; i < rows; i++)
                for (int j = 0; j < cols; j++)
                    Matrix[i, j] *= Escalar;
        }
        /// <summary>
        /// Obtiene la tangente sigmudea de un valor
        /// </summary>
        /// <param name="val">Valor al que se le va a aplicar la Tangente Sigmudea</param>
        /// <returns>La Tangente sigmudea</returns>
        public double tansigVal(double val)
        {
            return (2/(1+Math.Exp(-2.0*val))-1);
        }
        /// <summary>
        /// Le saca la tangente sigmudea a TODOS los elementos de la matriz contenida en este objeto
        /// </summary>
        public void TanSigMat()
        {
            for (int i = 0; i < rows; i++)
                for (int j = 0; j < cols; j++)
                    Matrix[i, j] = tansigVal(Matrix[i, j]);
        }
    }
}
