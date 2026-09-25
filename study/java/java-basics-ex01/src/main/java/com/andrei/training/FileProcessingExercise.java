package com.andrei.training;

import java.io.BufferedReader;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardOpenOption;
import java.util.*;

public class FileProcessingExercise {
    private final Path path = Paths.get("src/main/resources/input2.txt");

    public void run() {


        //long startTime = System.nanoTime();
        //long endTime = System.nanoTime();
        //long duration = (endTime - startTime) / 1_000_000;
        //System.out.println("Execution time for legacy BufferedReader: " + duration + " ms");

        System.out.println("=== Classic I/O ===");
        List<String> classicLines = readUsingClassicIO();
        printStats(classicLines);

        System.out.println("\n=== NIO Channel I/O ===");
        String nioString = readUsingNIOChannel();
        printStats(Arrays.asList(nioString.split(System.lineSeparator())));

    }

    private List<String> readUsingClassicIO() {
        List<String> lines = new ArrayList<>();

        try (BufferedReader reader = Files.newBufferedReader(path)) {
            String line;
            while ((line = reader.readLine()) != null) {
                lines.add(line);
            }
        } catch (IOException e) {
            throw new RuntimeException("Classic I/O failed", e);
        }
        return lines;
    }

    private String readUsingNIOChannel() {
        try (FileChannel fileChannel = FileChannel.open(path, StandardOpenOption.READ)) {

            ByteBuffer buffer = ByteBuffer.allocate(1024);
            StringBuilder builder = new StringBuilder();

            int bytes = fileChannel.read(buffer);

            while(bytes != -1) {
                buffer.flip();
                while (buffer.hasRemaining()) {
                    builder.append((char)buffer.get());
                }

                buffer.clear();
                bytes = fileChannel.read(buffer);
            }
            return  builder.toString();

        } catch (IOException e) {
            throw new RuntimeException("NIO Channel read failed", e);
        }
    }

    private void printStats(List<String> lines) {
        System.out.println("Total lines: " + lines.size());

        int charCount = lines.stream().mapToInt(String::length).sum();
        System.out.println("Total characters: " + charCount);

        Map<String, Integer> freq = new HashMap<>();
        for (String line : lines) {
            freq.put(line, freq.getOrDefault(line, 0) + 1);
        }

        System.out.println("Duplicate counts: " + freq);
    }
}
