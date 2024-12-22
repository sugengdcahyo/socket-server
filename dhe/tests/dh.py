from random import randrange

# Parameter: Prime and Generator
prime = 23  # Small prime for demonstration purposes
generator = 5  # Common generator

print(f"Initiate p :{prime} g: {generator}.", end="\n\n")

# Step 1: Private Keys
private_key_a = randrange(1, prime-1) # Alice's private key
private_key_b = randrange(1, prime-1)  # Bob's private key

print(f"Private key Alice: {private_key_a}")
print(f"Private key Bob: {private_key_b}", end="\n\n")

# Step 2: Public Keys
public_key_a = pow(generator, private_key_a, prime)  # Alice's public key: g^a mod p
public_key_b = pow(generator, private_key_b, prime)  # Bob's public key: g^b mod p

print(f"Public key Alice: {public_key_a}")
print(f"Public key Bob: {public_key_b}", end="\n\n")

# Step 3: Shared Secret
shared_secret_a = pow(public_key_b, private_key_a, prime)  # (g^b)^a mod p
shared_secret_b = pow(public_key_a, private_key_b, prime)  # (g^a)^b mod p

print(f"Shared key Alice: {shared_secret_a}")
print(f"Shared key Bob: {shared_secret_b}")

