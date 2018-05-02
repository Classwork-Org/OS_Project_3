import re

def main():
    f = open("outputRelease", "r")
    text = f.read()
    text = text.split("\n")
    buffer = []


    for eachline in text:
        print eachline

        producer = re.match('^producer produced (?P<item>[0-9]+)',eachline)
        consumer = re.match('^consumer consumed (?P<item>[0-9]+)',eachline)
        
        if(producer != None):
            itemToInsert = producer.group('item')
            buffer.append(itemToInsert)
            if(len(buffer) > 5):
                print "buffer size exceeded"
                return -1
            
        elif(consumer != None):
            itemToMatch = consumer.group('item')
            if(len(buffer) == 0):
                print "buffer size -1"
                return -1
            else:
                if(buffer[0] != itemToMatch):
                    print "wrong consumed item"
                    return -1
                else:
                    buffer.pop(0)

        print buffer


    print "all good!"


if __name__ == '__main__':
    main()