package com.andrei.training.animals;

import com.andrei.training.interfaces.Flyable;

public class Bird extends Animal implements Flyable {

    public Bird(String name, int age, float weight, String species) {
        super(name, age, weight, species);
    }

    @Override
    public void eat() {
        System.out.println("Bird: " + name + " is eating.");
    }

    @Override
    public void sleep() {
        System.out.println("Bird: " + name + " is sleeping.");
    }

    @Override
    public void call() {
        System.out.println("Bird: " + name + " is chirping.");
    }

    @Override
    public void printInfo() {
        System.out.printf("%s is a Bird.%n%d years old, weighs %.2f oz, and whose species is %s%n",name,age, weight, variation);
    }

    @Override
    public void fly() {
        System.out.println("Bird: " + name + " is flying.");
    }
}
