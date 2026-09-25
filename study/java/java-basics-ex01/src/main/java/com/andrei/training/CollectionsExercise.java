package com.andrei.training;

import java.nio.file.*;
import java.io.IOException;
import java.util.*;


public class CollectionsExercise {
    public void run() {
        try {

            Path path = Paths.get("src/main/resources/input.txt");

            List<String> names = Files.readAllLines(path);

            System.out.println("\nOriginal names:");
            System.out.println(names);
            System.out.println("Size: " + names.size());

            Set<String> uniqueNames = new HashSet<>(names);

            System.out.println("\nUnique names:");
            System.out.println(uniqueNames);
            System.out.println("Size: " + uniqueNames.size());

            List<String> sortedNames = new ArrayList<>(uniqueNames);
            Collections.sort(sortedNames);

            System.out.println("\nSorted names:");
            System.out.println(sortedNames);
            System.out.println("Size: " + sortedNames.size());

            Map<String, Integer> nameLengths = new HashMap<>();
            for (String name : sortedNames) {
                nameLengths.put(name, name.length());
            }

            System.out.println("\nName lengths:");
            System.out.println(nameLengths);

        } catch (IOException e) {
            System.out.println("Error reading file: " + e.getMessage());
        }
    }
}
