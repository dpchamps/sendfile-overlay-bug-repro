mkdir /tmp/copy_file_range_test
cd /tmp/copy_file_range_test

echo ""
echo "Create a new file, copy it (with cp) and then attempt to make copies with copy_file_range."
echo "  * A successful run results in four files, all with the same size."
echo "  * A repro produces four files, where testb-copy has zero bytes written to it."
echo ""

echo "hello" > testa
cp  testa testb

ls -Ali .
echo ""
IN_FILE=testa OUT_FILE=testa-copy /copy-file
IN_FILE=testb OUT_FILE=testb-copy /copy-file
echo""
ls -Ali .
