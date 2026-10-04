package com.andrei.training.animals;

import com.andrei.training.interfaces.Sprintable;

public class Cat extends Animal implements Sprintable {
    public Cat(String name, int age, float weight, String species) {
        super(name, age, weight, species);
    }

    @Override
    public void eat() {
        System.out.println("Cat: " + name + " is eating.");
    }

    @Override
    public void sleep() {
        System.out.println("Cat: " + name + " is sleeping.");
    }

    @Override
    public void call() {
        System.out.println("Cat: " + name + " is meowing.");
    }

    @Override
    public void printInfo() {
        System.out.printf("%s is a Cat.%n%d years old, weighs %.2f lbs, and whose breed is %s%n",name,age, weight, variation);
    }

    @Override
    public void Sprint() {
        System.out.println("Cat: " + name + " is sprinting.");
    }
}
