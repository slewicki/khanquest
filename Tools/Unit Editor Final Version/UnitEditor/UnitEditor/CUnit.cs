/// <summary>
/// File: “CUnit.cs”
/// Author: Sean Hamstra (SH)
/// Purpose: This file contains all of the statistical data of a unit.
/// </summary>

using System;
using System.Collections.Generic;
using System.Text;

/// <summary>
/// Enumeration of all unit types.
/// </summary>
public enum eUnitType
{
    INFANTRY, CAVALRY, CAVALRY_ARCHER, AXMEN, ARCHER,  WAR_ELEPHANT
};

namespace UnitEditor
{
    public class CUnit
    {
        // Fields
        int m_nHP = 0;
        int m_nCost = 0;

        public int Cost
        {
            get { return m_nCost; }
            set { m_nCost = value; }
        }
        int m_nAttack = 0;
        int m_nRange = 0;
        float m_fAttackSpeed = 0.0f;
        float m_fMovement = 0.0f;
        eUnitType m_eUnitType;
        int m_nHPMin;

        public int HPMin
        {
            get { return m_nHPMin; }
            set { m_nHPMin = value; }
        }
        int m_nHPMax;

        public int HPMax
        {
            get { return m_nHPMax; }
            set { m_nHPMax = value; }
        }

        int m_nAttackMin;

        public int AttackMin
        {
            get { return m_nAttackMin; }
            set { m_nAttackMin = value; }
        }
        int m_nAttackMax;

        public int AttackMax
        {
            get { return m_nAttackMax; }
            set { m_nAttackMax = value; }
        }

        int m_nRangeMin;

        public int RangeMin
        {
            get { return m_nRangeMin; }
            set { m_nRangeMin = value; }
        }
        int m_nRangeMax;

        public int RangeMax
        {
            get { return m_nRangeMax; }
            set { m_nRangeMax = value; }
        }

        int m_nCostMin;

        public int CostMin
        {
            get { return m_nCostMin; }
            set { m_nCostMin = value; }
        }
        int m_nCostMax;

        public int CostMax
        {
            get { return m_nCostMax; }
            set { m_nCostMax = value; }
        }


       
        float m_fAttackSpeedMax;

        public float AttackSpeedMax
        {
            get { return m_fAttackSpeedMax; }
            set { m_fAttackSpeedMax = value; }
        }
        float m_fAttackSpeedMin;

        public float AttackSpeedMin
        {
            get { return m_fAttackSpeedMin; }
            set { m_fAttackSpeedMin = value; }
        }
        float m_fMovementSpeedMax;

        public float MovementSpeedMax
        {
            get { return m_fMovementSpeedMax; }
            set { m_fMovementSpeedMax = value; }
        }
        float m_fMovementSpeedMin;

        public float MovementSpeedMin
        {
            get { return m_fMovementSpeedMin; }
            set { m_fMovementSpeedMin = value; }
        }
        




        
        /// <summary>
        /// Default constructor of CUnit
        /// </summary>
        public CUnit()
        {
        }

        /// <summary>
        /// Constructor of CUnit.
        /// </summary>
        /// <param name="nHP">The hitpoints of the unit.</param>
        /// <param name="nAttack">The attack power of the unit.</param>
        /// <param name="nRange">The range of the unit in tiles.</param>
        /// <param name="fAttackSpeed">The attack speed of the unit in seconds.</param>
        /// <param name="fMovement">The movement speed of the unit in tiles.</param>
        /// <param name="eUnitType">The type of the unit.</param>
        public CUnit(int nCost, int nHP, int nAttack, int nRange,
            float fAttackSpeed, float fMovement, eUnitType eUnitType)
        {
            Cost = nCost;
            HP = nHP;
            Attack = nAttack;
            Range = nRange;
            AttackSpeed = fAttackSpeed;
            Movement = fMovement;
            Type = eUnitType;
        }

        #region Properties
        /// <summary>
        /// The type of unit.
        /// </summary>
        public eUnitType Type
        {
            get { return m_eUnitType; }
            set { m_eUnitType = value; }
        }
        
        /// <summary>
        /// The hitpoints of the unit.
        /// </summary>
        public int HP
        {
            get { return m_nHP; }
            set { m_nHP = value; }
        }

        /// <summary>
        /// The attack power of the unit.
        /// </summary>
        public int Attack
        {
            get { return m_nAttack; }
            set { m_nAttack = value; }
        }

        /// <summary>
        /// The range of the unit.
        /// </summary>
        public int Range
        {
            get { return m_nRange; }
            set { m_nRange = value; }
        }

        /// <summary>
        /// The attack speed of the unit in seconds.
        /// </summary>
        public float AttackSpeed
        {
            get { return m_fAttackSpeed; }
            set { m_fAttackSpeed = value; }
        }

        /// <summary>
        /// The movement speed of the unit in tiles.
        /// </summary>
        public float Movement
        {
            get { return m_fMovement; }
            set { m_fMovement = value; }
        } 
        #endregion

        /// <summary>
        /// Overide the ToString of each unit to display the unit type in the list box.
        /// </summary>
        /// <returns>A string of the unit type.</returns>
        public override string ToString()
        {
            string szType = string.Empty;
            switch (this.Type)
            {
                case eUnitType.INFANTRY:
                    szType = "Infantry";
                    break;
                case eUnitType.CAVALRY:
                    szType = "Cavalry";
                    break;
                case eUnitType.CAVALRY_ARCHER:
                    szType = "Cavalry Archer";
                    break;
                case eUnitType.AXMEN:
                    szType = "Axmen";
                    break;
                case eUnitType.ARCHER:
                    szType = "Archer";
                    break;
                case eUnitType.WAR_ELEPHANT:
                    szType = "War Elephant";
                    break;
                default:
                    szType = "Invalid";
                    break;
            }
            return szType;
        }
    }
}
