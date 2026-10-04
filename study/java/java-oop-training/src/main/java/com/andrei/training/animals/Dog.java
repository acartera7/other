package com.andrei.training.animals;

import com.andrei.training.interfaces.Sprintable;
import com.andrei.training.interfaces.Swimmable;

public class Dog extends Animal implements Swimmable, Sprintable {

    public Dog(String name, int age, float weight, String species) {
        super(name, age, weight, species);
    }

    @Override
    public void eat() {
        System.out.println("Dog: " + name + " is eating.");
    }

    @Override
    public void sleep() {
        System.out.println("Dog: " + name + " is sleeping.");
    }

    @Override
    public void call() {
        System.out.println("Dog: " + name + " is barking.");
    }

    @Override
    public void printInfo() {
        System.out.printf("%s is a Dog.%n%d years old, weighs %.2f lbs, and whose breed is %s%n",name,age, weight, variation);
    }

    @Override
    public void Sprint() {
        System.out.println("Dog: " + name + " is sprinting.");
    }

    @Override
    public void swim() {
        System.out.println("Dog: " + name + " is swimming.");
    }
}
