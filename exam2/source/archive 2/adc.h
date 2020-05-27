void ADC_init()
{
    ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}
