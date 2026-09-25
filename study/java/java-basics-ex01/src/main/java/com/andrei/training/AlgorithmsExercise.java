package com.andrei.training;

import java.io.IOException;
import java.nio.file.*;
import java.util.*;

public class AlgorithmsExercise {
    public void run() {
        try {
            Path path = Paths.get("src/main/resources/input.txt");
            List<String> names = Files.readAllLines(path);

            Map<String, Integer> occurrences = new HashMap<>();

            for(String name : names) {
                occurrences.compute(name, (k, v) -> (v == null) ? 1 : v+1);
            }

            System.out.println("Occurrences of names:");
            System.out.println(occurrences);

            List<String> reversedNames = new ArrayList<>();
            for(String name : names.reversed()) {
                reversedNames.add(name);
            }

            System.out.println("Reversed names:");
            System.out.println(reversedNames);

            List<String> longNames = new ArrayList<>();
            for(String name : names) {
                if( name.length() > 4) {
                    longNames.add(name);
                }
            }

            System.out.println("Long names:");
            System.out.println(longNames);



        } catch (IOException e) {
            System.out.println("Error reading file: " + e.getMessage());
        }
    }
}
