package com.andrei.training.exercises;

import java.util.*;
import com.andrei.training.animals.*;
import com.andrei.training.interfaces.Flyable;

public class Exercise7 {
    public static void run() {
        List<Animal> animals = List.of(
                new Dog("Max", 3, 26, "Beagle"),
                new Cat("Sparkles", 5, 12, "Ragdoll"),
                new Bird("Pipsqueak", 2, 1.2f, "Budgerigar"),
                new Cat("Molly", 4, 13.5f, "Persian"),
                new Dog("Judge", 5, 50, "English Bulldog")
        );

        for (Animal animal : animals) {
            animal.printInfo();
            animal.call();
        }

        Animal a1 = new Dog("Max", 3, 26, "Beagle");

        System.out.println("\nAnimal a1: ");
        a1.printInfo();

        System.out.println("Is a1 equal to the first animal in the prior list?");
        System.out.println(a1.equals(animals.getFirst()));

        System.out.println("Is a1 able to fly? (a1 instanceof Flyable) :");
        System.out.println(a1 instanceof Flyable);


    }
}
