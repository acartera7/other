package com.andrei.training;

import java.io.BufferedReader;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.*;
import java.util.function.Function;
import java.util.stream.Collectors;

public class StreamExercise {

    private final List<String> lines = new ArrayList<>();

    public StreamExercise() {
        Path path = Paths.get("src/main/resources/input2.txt");
        try (BufferedReader reader = Files.newBufferedReader(path)) {
            String line;
            while ((line = reader.readLine()) != null) {
                lines.add(line);
            }
        } catch (IOException e) {
            throw new RuntimeException("Classic I/O failed", e);
        }
    }

    public void run() {
        System.out.println("=== Filter lines that start with \"INFO:\" ===");
        System.out.println(filterLines());

        System.out.println("\n=== Convert to Uppercase ===");
        System.out.println(transformUpper());

        System.out.println("\n=== Sort Alphabetically ===");
        System.out.println(sortAlphabetically());

        System.out.println("\n=== Remove Duplicates ===");
        System.out.println(removeDuplicates());

        System.out.println("\n=== Count lines containing \"cache\" ===");
        System.out.println(countCache());

        System.out.println("\n=== Frequency Map ===");
        System.out.println(getFrequencyMap());
    }

    private List<String> filterLines() {
        return lines.stream()
                    .filter((String s) -> s.startsWith("INFO:"))
                    .collect(Collectors.toList());
    }

    private List<String> transformUpper() {
        return lines.stream()
                    .map(String::toUpperCase)
                    .collect(Collectors.toList());
    }

    private List<String> sortAlphabetically() {
        return lines.stream()
                    .sorted()
                    .collect(Collectors.toList());
    }

    private List<String> removeDuplicates() {
        return lines.stream()
                    .distinct()
                    .collect(Collectors.toList());
    }
    private long countCache() {
        return lines.stream()
                    .filter(s -> s.toLowerCase(Locale.ROOT).contains("cache"))
                    .count();
    }

    private Map<String, Long> getFrequencyMap() {
        return lines.stream().collect(Collectors.groupingBy(
                Function.identity(),
                Collectors.counting()
        ));
    }
}
