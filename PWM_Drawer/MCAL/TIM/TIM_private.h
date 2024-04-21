/**
 ******************************************************************************
 * @file    TIM_private.h
 * @author  Salma Faragalla
 ******************************************************************************
 */

#ifndef TIM_TIM_PRIVATE_H_
#define TIM_TIM_PRIVATE_H_

#define TIM_CLK (8000000UL)
#define TIM_MAX_PERIOD (0xFFFF)


typedef struct
{
	volatile u32 CR1;
	volatile u32 CR2  ;
	volatile u32 SMCR;
	volatile u32 DIER ;
	volatile u32 SR ;
	volatile u32 EGR;
	volatile u32 CCMR1;
	volatile u32 CCMR2;
	volatile u32 CCER;
	volatile u32 CNT ;
	volatile u32 PSC;
	volatile u32 ARR ;
	volatile u32 RCR;
	volatile u32 CCR1 ;
	volatile u32 CCR2 ;
	volatile u32 CCR3 ;
	volatile u32 CCR4  ;
	volatile u32 BDTR;
	volatile u32 DCR  ;
	volatile u32 DMAR  ;

}TIM_TypeDef;

#define TIM1 ((volatile TIM_TypeDef*)0x40012C00UL)
#define TIM2 ((volatile TIM_TypeDef*)0x40000000UL)
#define TIM3 ((volatile TIM_TypeDef*)0x40000400UL)


/* TIM Register Pins*/

/* TIMx_CR1 */
#define CR1_CEN (0UL)
#define CR1_ARPE (7UL)

/* TIMx_EGR */
#define EGR_UG (0UL)
#define EGR_CC1G (1UL)
#define EGR_CC2G (2UL)
#define EGR_CC3G (3UL)
#define EGR_CC4G (3UL)

/* TIMx_CCMR1 */
#define CCMR1_CC1S (0UL)
#define CCMR1_CC2S (8UL)

/* TIMx_CCMR1 Output Compare Mode*/
#define CCMR1_OC1FE (2UL)
#define CCMR1_OC1PE (3UL)
#define CCMR1_OC1M (4UL)
#define CCMR1_OC1CE (7UL)

#define CCMR1_OC2FE (10UL)
#define CCMR1_OC2PE (11UL)
#define CCMR1_OC2M (12UL)
#define CCMR1_OC2CE (15UL)

/* TIMx_CCMR2 */
#define CCMR2_CC3S (0UL)
#define CCMR2_CC4S (8UL)

/* TIMx_CCMR1 Output Compare Mode*/
#define CCMR2_OC3FE (2UL)
#define CCMR2_OC3PE (3UL)
#define CCMR2_OC3M (4UL)
#define CCMR2_OC3CE (7UL)

#define CCMR2_OC4FE (10UL)
#define CCMR2_OC4PE (11UL)
#define CCMR2_OC4M (12UL)
#define CCMR2_OC4CE (15UL)

/* OCM Modes */
#define OCM_TIMING (0UL)
#define OCM_ACTIVE (1UL)
#define OCM_INACTIVE (2UL)
#define OCM_TOGGLE (3UL)
#define OCM_FORCED_INACTIVE (4UL)
#define OCM_FORCED_ACTIVE (5UL)
#define OCM_PWM1 (6UL)
#define OCM_PWM2 (7UL)

/* TIMx_CCER */
#define CCER_CC1E (0UL)
#define CCER_CC1P (1UL)
#define CCER_CC1NE (2UL)
#define CCER_CC1NP (3UL)

#define CCER_CC2E (4UL)
#define CCER_CC2P (5UL)
#define CCER_CC2NE (6UL)
#define CCER_CC2NP (7UL)

#define CCER_CC3E (8UL)
#define CCER_CC3P (9UL)
#define CCER_CC3NE (10UL)
#define CCER_CC3NP (11UL)

#define CCER_CC4E (12UL)
#define CCER_CC4P (13UL)
#define CCER_CC4NE (14UL)
#define CCER_CC4NP (15UL)

/* TIMx_BDTR */
#define BDTR_MOE (15UL)

/* @defgroup CCS_DIRECTION */
#define CCS_OP (0UL)
#define CCS_IP_DIRECT (1UL)
#define CCS_IP_INDIRECT (2UL)
#define CCS_IP_TRC (3UL)

/* TIMx_CCMRx Input Mode*/
#define CCMR1_IC1F (4UL)
#define CCMR1_IC2F (12UL)

/* TIMx_DIER */
#define DIER_CC1IE (1UL)
#define DIER_CC2IE (2UL)
#define DIER_CC3IE (3UL)
#define DIER_CC4IE (4UL)

/* TIMx_SMCR */

#define SMCR_SMS (0UL)
#define SMCR_TS  (4UL)
#define SMCR_MSM (7UL)
#define SMCR_ETF (8UL)
#define SMCR_ETPS (12UL)
#define SMCR_ECE (14UL)
#define SMCR_ETP (15UL)

/* Trigger selection */
#define TS_ITR0   (0UL)
#define TS_ITR1   (1UL)
#define TS_ITR2    (2UL)
#define TS_ITR3    (3UL)
#define TS_TI1F_ED (4UL)
#define TS_TI1FP1  (5UL)
#define TS_TI2FP2   (6UL)
#define TS_ETRF     (1UL)

/* Slave mode selection */
#define SMS_DISABLE  (0UL)
#define SMS_ENC1	(1UL)
#define SMS_ENC2    (2UL)
#define SMS_ENC3    (3UL)
#define SMS_RESET    (4UL)
#define SMS_GATED    (5UL)
#define SMS_TRIGGER (6UL)
#define SMS_EXT1     (7UL)


/* TIMx_SR*/
#define SR_CC1IF (1UL)
#define SR_CC2IF (2UL)
#define SR_CC3IF (3UL)
#define SR_CC4IF (4UL)
#endif /* TIM_TIM_PRIVATE_H_ */
