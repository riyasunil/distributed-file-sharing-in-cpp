# Distributed File Orchestration and Synchronization: Multi-Node Data-Transfer-Framework for Linux
### Overview:

Design and implement a multi-client file transfer system using a client-server model in C, python, or Golang, where the server can handle multiple clients simultaneously. The server should authenticate clients, allow them to upload, download, view, and delete files from a server-side directory, and respond to multiple concurrent requests without crashing or losing data.

### Background:

Modern file transfer systems, such as FTP, are widely used for transferring data between clients and servers in networked environments. Such systems need to support various functionalities like user authentication, file uploading, downloading, and secure data management. Additionally, the system must handle multiple clients simultaneously without sacrificing performance or data integrity.

### Problem Statement:

You are tasked with implementing a multi-client file transfer system that will:

1. **Authenticate clients** based on a predefined list of usernames and passwords.
2. Allow authenticated clients to perform the following actions:
    - **Upload Files**: Clients should be able to upload files to their specific directory on the server.
    - **Download Files**: Clients should be able to download files that exist in their directory on the server.
    - **View Files**: Clients can preview the first 1024 bytes of any file in their directory.
    - **Delete Files**: Clients can delete any file within their directory on the server.
    - **List Files**: Clients should be able to request a list of all files stored in their specific directory.
3. **Handle multiple clients concurrently**, without interference between them.
4. Support a robust **signal handling mechanism** that ensures the **server can safely shut down while maintaining data integrity**.

### Functional Requirements:

1. **Client Authentication**:
    - The server maintains a list of valid username-password pairs in a file (`id_passwd.txt`).
    - Upon connecting, clients must provide a valid username and password. If the credentials are incorrect, the server should reject the client and terminate the session.
2. **File Management**:
    - Clients should be able to:
        - **Upload Files**: After authenticating, a client can upload a file to the server by providing the file name. The server should save the file in a directory specific to the client (e.g., `/server_storage/<username>`).
        - **Download Files**: The client can request a file to download from their directory on the server. If the file exists, the server should send it; otherwise, an error message is returned.
        - **View Files**: The client can request a preview of the first 1024 bytes of any file in their directory.
        - **Delete Files**: The client can delete any file from their directory. Upon successful deletion, the server should confirm the operation.
        - **List Files**: Clients can request a list of all files stored in their directory. The server should send a list of file names in that directory.
3. **Concurrency and Resource Management**:
    - The server must handle multiple clients simultaneously. When a new client connects, the server should fork a new process or use threading (if desired) to handle the client’s requests independently, without blocking other clients from connecting.
    - The server must handle signals (e.g., SIGINT) to close open sockets gracefully and ensure that any active client connections are properly terminated upon server shutdown.
4. **Robust Error Handling**:
    - Ensure that errors such as file not found, invalid commands, or communication failures are handled gracefully and communicated clearly to the client.
    - Invalid file paths should not allow clients to escape their designated directories or access unauthorized files.
5. **Security and Data Integrity**:
    - The system should ensure that clients can only access their own files. Each client should have a dedicated directory (e.g., `/server_storage/<username>`) that no other client can access.
    - Handle possible race conditions or conflicts that could arise from multiple clients accessing the server concurrently.
    - Consider potential security risks such as buffer overflows or directory traversal attacks and mitigate them accordingly.

### Non-Functional Requirements:

1. **Scalability**:
    - The system should be designed to handle at least 2 concurrent clients without noticeable degradation in performance.
2. **Maintainability**:
    - The code should be modular, well-commented, and designed in a way that additional features like encryption or more complex authentication mechanisms can be added easily in the future.
3. **Reliability**:
    - The server must guarantee that file uploads and downloads are completed fully and accurately, even if multiple clients are performing these actions at the same time.
    - If the server crashes or is interrupted (e.g., by a SIGINT signal), all resources (sockets, file descriptors) should be closed properly.
4. **Efficiency**:
    - The server should minimize CPU and memory usage, even when handling multiple clients.
    - Data transfers between clients and the server should be optimized to minimize delays, especially for large files.

### Constraints:

1. **Programming Language**: The server must be implemented in C, Python, or Golang. (suggested to use Unix-based socket programming).
2. **Platform**: The system will be deployed on a Unix-based system (e.g., Linux).
3. **Security**: For this phase of development, simple authentication (username/password) without encryption is acceptable, but the system must be designed with future security improvements in mind.

### Assumptions:

1. The server will have access to a file (`id_passwd.txt`) that contains valid username-password pairs for client authentication.
2. All clients accessing the server will use the same protocol for communicating with the server (TCP-based).
3. Each client will have their own directory on the server where files are stored (e.g., `/server_storage/<username>`).

### Challenges:

1. **Concurrency**: Managing multiple clients concurrently, ensuring that one client’s file operations do not interfere with another’s.
2. **Data Integrity**: Guaranteeing that file transfers are complete and accurate, even in the case of concurrent uploads/downloads.
3. **Error Handling**: Handling unexpected conditions (e.g., invalid commands, missing files) gracefully and without crashing the server.
4. **Security**: Preventing unauthorized access to files and directories through proper path management and validation.
5. **Signal Handling**: Ensuring that the server can handle signals like `SIGINT` (Ctrl+C) for safe shutdown, closing all open sockets, and ensuring data consistency.

<aside>
💡

**Signal Handling**: If the client presses Ctrl+C, something something something which shouldn’t be the case, like client presses ctrl+C and on the server side it’s still listening for the client but client stops, something like this

</aside>

### Expected Outcome:

By the end of the project, the server should be able to:

- Authenticate multiple clients concurrently.
- Allow each authenticated client to upload, download, view, and delete files in their directory.
- List the contents of a client’s directory.
- Safely handle multiple concurrent connections without data loss or corruption.
- Shut down gracefully upon receiving an interrupt signal, ensuring all resources are properly cleaned up.

---

### Extensions:

As part of future development, the following features can be added:

- **SSL/TLS Support**: For secure communication between the client and server.
- **Logging and Auditing**: To track client actions and server performance over time.
- **Thread-based Implementation**: Instead of forking processes, a thread-based approach could be explored for more efficient resource management.

## FTP Server and Client `Setup` Guide

#### Building the Executables
1. Organize the server.c, common.c, and the Makefile in a folder [X] on PC1.
2. Organize client.c, common.c, and the Makefile in a folder [Y] on PC2.
3. Navigate to the respective folder (X/Y) and execute `make` to build server.out and client.out files.
   
#### Setting Up the Server.
```
make
```
1. `a.out` is created in the server folder.
2. Create a file named "id_passwd.txt" in folder [X], containing usernames and passwords of multiple clients.
3. Each entry in id_passwd.txt should follow the format:
```
[userName]/:[password]
```
4. In the same folder [X], create an empty directory with the same name as each client's username for every user.
5. Launch the server by running `./server.out [ServerPortNo]`.
```
./server.out [ServerPortNo]
```

#### Setting Up the Client

1. Only the client.out file is required in the client computer's folder [Y].
2. To run the client, navigate to folder [Y] and execute `./client.out [ServerIP] [ServerPort]`.
```
./client.out [ServerIP] [ServerPort]
```

