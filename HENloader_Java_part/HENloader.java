/*
 * Copyright (c) 2025 BenNox_XD
 *
 * This file is part of PS5-BDJ-HEN-loader and is licensed under the MIT License.
 * See the LICENSE file in the root of the project for full license information.
 */
// Original src from Dr.Oid https://x.com/RepassyMate
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import org.ps5jb.loader.Status;

public class HENloader {
    private static final String SERVER_IP = "127.0.0.1";
    private static final int SERVER_PORT = 9021;
    private static final int BUFFER_SIZE = 8192;
    private static final String FILE = "/HENloader.elf";

    private static void println(String message) {
        Status.println(message);
    }

    public static void main(String[] args) {
        println("Starting...");

        println("Host: " + SERVER_IP + ":" + SERVER_PORT);

        try (
            Socket socket = new Socket(SERVER_IP, SERVER_PORT);
            InputStream inputStream = HENloader.class.getResourceAsStream(FILE);
            OutputStream outputStream = socket.getOutputStream()
        ) {
            if (inputStream == null) {
                throw new IOException("Cannot find file " + FILE + " in JAR");
            }

            println("Connection successful!");
            println("Loading file: " + FILE);

            byte[] buffer = new byte[BUFFER_SIZE];
            int bytesRead;
            int totalBytesSent = 0;

            while ((bytesRead = inputStream.read(buffer)) != -1) {
                outputStream.write(buffer, 0, bytesRead);
                totalBytesSent += bytesRead;
            }

            outputStream.flush();
            println("All good, sent: " + totalBytesSent + " byte" + (totalBytesSent == 1 ? "" : "s"));
            println(FILE + " sent!");
        } catch (IOException e) {
            println("An error occurred: " + e.getMessage());
            e.printStackTrace();
        }

        println("Finished successfully!");
    }
}