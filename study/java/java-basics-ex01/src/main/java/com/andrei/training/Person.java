package com.andrei.training;

public class Person {
    private String name;
    private int age;

    public Person(String name, int age) {
        this.name = name;
        this.age = age;
    }

    public String greet() {
        return "Hello, my name is " + name + " and I am " + age + " years old.";
    }

    public int getBirthYear(int currentYear) {
        return currentYear - age;
    }
}
