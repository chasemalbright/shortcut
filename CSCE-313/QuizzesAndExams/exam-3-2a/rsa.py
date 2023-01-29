from Crypto.PublicKey import RSA
from Crypto.Cipher import PKCS1_OAEP
from Crypto.Hash import SHA256
from Crypto.Signature import pkcs1_15
import os

# ======== support functions for part b and d ========
def find_decrypt_key(message, dir="key_pairs/"):
    # iterate through all the keys in the directory
    for file in os.listdir(dir):
        if not file.endswith(".pem"): # pem files only
            continue # skip to the next file
        elif decrypt_message(message, dir + file, "temp.txt"): # if the decryption is successful
            os.remove("temp.txt") # remove the temporary file
            return dir + file # return the private key file name

def find_sign_key(message, signature, dir="key_pairs/"): # resuse find and pass signature
    # [IMPLEMENT THIS FUNCTION TO RETURN THE PATH TOWARDS THE PUBLIC KEY]
    for file in os.listdir(dir):
        if not file.endswith(".pem"): 
            continue # skip to the next file
        elif verify_message(message, signature, dir + file): # if the decryption is successful
            
            return dir + file # return the private key file name

# ======== main functions ========
# Generate a public/private key pair using 2048 bits key length
def generate_keys(public_fname="public.pem", private_fname="private.pem"):
    # generate the key pair
    key = RSA.generate(2048)

    # ======= public key =======
    public_key = key.public_key().export_key()
    public = open(public_fname, "wb")
    public.write(public_key)
    public.close()

    # ======= private key =======
    private_key = key.export_key()
    private = open(private_fname, "wb")
    private.write(private_key)
    private.close()


# Encrypt a message using a public key
def encrypt_message(message, pub_key_path, out_fname="encrypted.txt"):

    f = open(out_fname, "wb")

    key = RSA.import_key(open(pub_key_path).read())

    cipher_RSA = PKCS1_OAEP.new(key)

    cipher_text = cipher_RSA.encrypt(message)

    f.write(cipher_text)

    f.close()

# Decrypt a message using a private key
def decrypt_message(message, priv_key_path, out_fname="decrypted.txt"):

    out = open(out_fname, "wb")

    # decrypt the message with the private RSA key using PKCS1_OAEP
    # and return True if the decryption is successful
    try:
        key = RSA.import_key(open(priv_key_path).read())

        cipher = PKCS1_OAEP.new(key)

        cipher_text = cipher.decrypt(message)

        out.write(cipher_text)

        out.close()

        # return True if decryption is successful
        print("The private key is valid.")
        return True
    
    except (ValueError, TypeError):
        # return False if decryption is unsuccessful
        print("The private key is invalid.")
        return False

# Sign a message using a private key
def sign_message(message, priv_key_path, out_fname="signed_msg.txt"):

    f = open(out_fname, "wb")



    key = RSA.import_key(open(priv_key_path).read())


    sh = SHA256.new(message)
    key = RSA.importKey(open(priv_key_path).read())

    cipher = pkcs1_15.new(key)
    cipher_text = cipher.sign(sh)
    f.write(cipher_text)
    f.close()

# Verify a message using a public key
def verify_message(message, signature, public_key_path):
    key = RSA.import_key(open(public_key_path).read())

    sh = SHA256.new(message)


    # verify the signature with the public RSA key using pkcs1_15
    try:
        pkcs1_15.new(key).verify(sh, signature) #verify docutmation for fucntion signature
        print("The signature is valid.")
        return True
    except (ValueError, TypeError): # add type error to catch other exception
        print("The signature is not valid.")
        return False


def print_menu():
    """Prints the menu of options"""
    print("*******************Main Menu*******************")
    print('a. Generate public and private keys')
    print('b. Find the right key and decrypt the message in sus.txt')
    print('c. Sign a message and verify it')
    print('d. Find Miss Reveille\'s key pair that she used to sign rev.txt')
    print('q. Quit')
    print('***********************************************\n')

if __name__ == "__main__":
    while True:
        print_menu()
        option = input('Choose a menu option: ')
        if option == "a":
            # part a.1: generate public and private keys
            generate_keys()
           
            # part a.2: ask a message to be encrypted and encrypt it
            message = input("Enter a message to be encrypted: ")
            message = message.encode()
            public_key_path = "public.pem"
            
            encrypt_message(message, public_key_path) # encrypted.txt

            # part a.3: decrypt that exact message and output it to a file 
            #           called decrypted.txt
            private_key_path = "private.pem"
            encrypted_message = open("encrypted.txt", "rb").read()
            decrypt_message(encrypted_message, private_key_path)
            
        elif option == "b":
            # part b: decrypt the message given in sus.txt using one of the keys in key_pairs
            #         and output the decrypted message to a file called sus_decrypted.txt
            #         HINT: use the find_decrypt_key function to your advantage
            message = open("sus.txt", "rb").read()


            decrypt_file = find_decrypt_key(message)
            
            decrypt_message(message, decrypt_file,"sus_decrypted.txt")

        elif option == "c":
            # part c.1: sign a message using the private key from part a.1
            #           and export the signature to a file called signed_msg.txt
            message = input("Enter a message to be signed: ")
            message = message.encode()
            private_key_path = "private.pem"
            sign_message(message, private_key_path)

            # part c.2: verify the signature of the message using 
            #           the public key from part a.1 
            public_key_path = "public.pem"
            signature = open("signed_msg.txt", "rb").read()
            verify_message(message, signature, public_key_path)
        
        elif option == "d":
            # part d: identify the real Reveille's signature
            #         by verifying the signature of the message in 
            #         sus_decrypted.txt
            #         HINT:
            #         - think about how to find the correct key IRL (trial and error)
            #         - you are more than welcome to write a helper function to find the key
            #           and if you do, you can write find_sign_key() function
            #         - whatever method you use, as long as we select this option and get the
            #           correct key, you will get full credit
            message = open("sus_decrypted.txt", "rb").read()
            signature = open("rev.txt", "rb").read()
            file = find_sign_key(message, signature)
            print("The signature was found in: " + file)
            
        elif option == "q":
            break
