package com.andrei.training.strategyanimals;

public abstract class StrategyAnimal {
    protected String name;
    protected int age;
    protected float weight;
    protected String variation;

    public String getName() {
        return name;
    }

    public int getAge() {
        return age;
    }

    public float getWeight() {
        return weight;
    }

    public String getVariation() {
        return variation;
    }

    public StrategyAnimal(String name, int age, float weight, String variation) {
        this.name = name;
        this.age = age;
        this.weight = weight;
        this.variation = variation;
    }
}
