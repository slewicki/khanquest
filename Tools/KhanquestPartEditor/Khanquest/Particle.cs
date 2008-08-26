/// <summary>
/// File: “Particle.cs”
/// Author: Rodney Kite (RK)
/// Date Created	:	7/17/2008
/// Purpose: This class  holds all of the information for a single particle.
/// </summary>
using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

namespace Khanquest
{
    /// <summary>
    /// All of the data members that each particle has
    /// </summary>
    public class Particle
    {

        #region Fields
        /// <summary>
        /// Class members
        /// </summary>
        bool alive = true;
        bool continuous = true;
        decimal locX = 0;
        decimal locY = 0;
        int offsetX = 0;
        int offsetY = 0;
        decimal maxParticles = 10;
        decimal life = 1;
        decimal maxLife = 1;
        decimal minLife = 1;
        decimal age = 0;

        decimal velX = 0;
        decimal velY = 0;

        decimal startAlpha = 0;
        decimal startRed = 0;
        decimal startGreen = 0;
        decimal startBlue = 0;

        decimal currentAlpha = 0;
        decimal currentRed = 0;
        decimal currentGreen = 0;
        decimal currentBlue = 0;

        decimal endAlpha = 0;
        decimal endRed = 0;
        decimal endGreen = 0;
        decimal endBlue = 0;

        decimal startScaleX = 0;
        decimal currentScaleX = 0;
        decimal endScaleX = 0;

        decimal startScaleY = 0;
        decimal currentScaleY = 0;
        decimal endScaleY = 0;

        decimal gravityX = 0;
        decimal gravityY = 0;

        string sourceBlend = string.Empty;
        string destBlend = string.Empty;
        string trigger = string.Empty;
        string fileName = string.Empty;
        int imageID = -1;

        #endregion

        /// <summary>
        /// The Particle Constructor
        /// </summary>
        /// <returns>void</returns>
        public Particle()
        {

        }

        #region Events

        #endregion

        #region Properties

        /// <summary>
        /// Properties
        /// </summary>
        /// /// <param name="value">the value to change</param>
        /// <returns>Particle member value</returns>
        public bool Alive
        {
            get
            {
                return alive;
            }
            set
            {
                this.alive = value;
            }
        }
        public bool Continuous
        {
            get
            {
                return continuous;
            }
            set
            {
                this.continuous = value;
            }
        }
        public decimal LocX
        {
            get
            {
                return locX;
            }
            set
            {
                this.locX = value;
            }
        }
        public decimal LocY
        {
            get
            {
                return locY;
            }
            set
            {
                this.locY = value;
            }
        }
        public int OffsetX
        {
            get
            {
                return offsetX;
            }
            set
            {
                this.offsetX = value;
            }
        }
        public int OffsetY
        {
            get
            {
                return offsetY;
            }
            set
            {
                this.offsetY = value;
            }
        }
        public decimal Age
        {
            get
            {
                return age;
            }
            set
            {
                this.age = value;
            }
        }
        public decimal MaxParticles
        {
            get
            {
                return maxParticles;
            }
            set
            {
                this.maxParticles = value;
            }
        }
        public decimal Life
        {
            get
            {
                return life;
            }
            set
            {
                this.life = value;
            }
        }
        public decimal MaxLife
        {
            get
            {
                return maxLife;
            }
            set
            {
                this.maxLife = value;
            }
        }
        public decimal MinLife
        {
            get
            {
                return minLife;
            }
            set
            {
                this.minLife = value;
            }
        }
        public decimal VelX
        {
            get
            {
                return velX;
            }
            set
            {
                this.velX = value;
            }
        }
        public decimal VelY
        {
            get
            {
                return velY;
            }
            set
            {
                this.velY = value;
            }
        }
        public string SourceBlend
        {
            get
            {
                return sourceBlend;
            }
            set
            {
                this.sourceBlend = value;
            }
        }
        public string DestBlend
        {
            get
            {
                return destBlend;
            }
            set
            {
                this.destBlend = value;
            }
        }
        public decimal CurrentRed
        {
            get
            {
                return currentRed;
            }
            set
            {
                this.currentRed = value;
            }
        }
        public decimal CurrentBlue
        {
            get
            {
                return currentBlue;
            }
            set
            {
                this.currentBlue = value;
            }
        }
        public decimal CurrentGreen
        {
            get
            {
                return currentGreen;
            }
            set
            {
                this.currentGreen = value;
            }
        }
        public decimal StartAlpha
        {
            get
            {
                return startAlpha;
            }
            set
            {
                this.startAlpha = value;
            }
        }
        public decimal CurrentAlpha
        {
            get
            {
                return currentAlpha;
            }
            set
            {
                this.currentAlpha = value;
            }
        }
        public decimal EndAlpha
        {
            get
            {
                return endAlpha;
            }
            set
            {
                this.endAlpha = value;
            }
        }
        public decimal StartRed
        {
            get
            {
                return startRed;
            }
            set
            {
                this.startRed = value;
            }
        }
        public decimal EndRed
        {
            get
            {
                return endRed;
            }
            set
            {
                this.endRed = value;
            }
        }
        public decimal StartGreen
        {
            get
            {
                return startGreen;
            }
            set
            {
                this.startGreen = value;
            }
        }
        public decimal EndGreen
        {
            get
            {
                return endGreen;
            }
            set
            {
                this.endGreen = value;
            }
        }
        public decimal StartBlue
        {
            get
            {
                return startBlue;
            }
            set
            {
                this.startBlue = value;
            }
        }
        public decimal EndBlue
        {
            get
            {
                return endBlue;
            }
            set
            {
                this.endBlue = value;
            }
        }
        public decimal StartScaleX
        {
            get
            {
                return startScaleX;
            }
            set
            {
                this.startScaleX = value;
            }
        }
        public decimal StartScaleY
        {
            get
            {
                return startScaleY;
            }
            set
            {
                this.startScaleY = value;
            }
        }
        public decimal CurrentScaleX
        {
            get
            {
                return currentScaleX;
            }
            set
            {
                this.currentScaleX = value;
            }
        }
        public decimal CurrentScaleY
        {
            get
            {
                return currentScaleY;
            }
            set
            {
                this.currentScaleY = value;
            }
        }
        public decimal EndScaleX
        {
            get
            {
                return endScaleX;
            }
            set
            {
                this.endScaleX = value;
            }
        }
        public decimal EndScaleY
        {
            get
            {
                return endScaleY;
            }
            set
            {
                this.endScaleY = value;
            }
        }

        public decimal GravityX
        {
            get
            {
                return gravityX;
            }
            set
            {
                this.gravityX = value;
            }
        }

        public decimal GravityY
        {
            get
            {
                return gravityY;
            }
            set
            {
                this.gravityY = value;
            }
        }
        public string Trigger
        {
            get
            {
                return trigger;
            }
            set
            {
                this.trigger = value;
            }
        }
        public int ImageID
        {
            get
            {
                return imageID;
            }
            set
            {
                this.imageID = value;
            }
        }
        public string FileName
        {
            get
            {
                return fileName;
            }
            set
            {
                this.fileName = value;
            }
        }

        #endregion

        #region Methods

        #endregion
    }
}
