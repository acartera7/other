package com.andrei.training.animals;

import java.util.Objects;


public abstract class Animal {
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

    public Animal(String name, int age, float weight, String variation) {
        this.name = name;
        this.age = age;
        this.weight = weight;
        this.variation = variation;
    }

    public abstract void eat();
    public abstract void sleep();
    public abstract void call();
    public abstract void printInfo();

    @Override
    public boolean equals(Object o) {
        if (o == null || getClass() != o.getClass()) return false;
        Animal animal = (Animal) o;
        return  age == animal.age &&
                Float.compare(weight, animal.weight) == 0 &&
                Objects.equals(name, animal.name) &&
                Objects.equals(variation, animal.variation);
    }

    @Override
    public int hashCode() {
        return Objects.hash(name, age, weight, variation);
    }
}
