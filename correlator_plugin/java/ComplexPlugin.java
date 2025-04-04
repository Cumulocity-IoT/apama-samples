// Copyright (c) 2013-present Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.
// Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH.

@com.apama.epl.plugin.annotation.ApplicationJar(
	name = "ComplexPluginJar",
	classpath = ""
)

/**
  A more complex correlator Java plugin with multiple methods, arrays and chunks.
*/
@com.apama.epl.plugin.annotation.EPLPlugin(
	name="ComplexPlugin",
	description="A test plugin"
)
public class ComplexPlugin
{
	/** Prints the arguments and returns a string */
	public static String test1(long l, double f, boolean b, String s, Number d)
	{
		System.out.println("integer value = "+l);
		System.out.println("float value = "+f);
		System.out.println("boolean value = "+b);
		System.out.println("string value = "+s);
		System.out.println("decimal value = "+d);
		return "Hello, World";
	}
	
	/** Prints the (array) arguments and returns a string */
	public static double test2(long[] ls, double[] fs, boolean[] bs, String[] ss, Number[] sd)
	{
		System.out.println("sequence size = "+ls.length);
		for (int i = 0; i < ls.length; ++i) {
			System.out.print("sequence element["+i+"]: ");
			System.out.println("integer value = "+ls[i]);
		}
		System.out.println("sequence size = "+fs.length);
		for (int i = 0; i < fs.length; ++i) {
			System.out.print("sequence element["+i+"]: ");
			System.out.println("float value = "+fs[i]);
		}
		System.out.println("sequence size = "+bs.length);
		for (int i = 0; i < bs.length; ++i) {
			System.out.print("sequence element["+i+"]: ");
			System.out.println("boolean value = "+bs[i]);
		}
		System.out.println("sequence size = "+ss.length);
		for (int i = 0; i < ss.length; ++i) {
			System.out.print("sequence element["+i+"]: ");
			System.out.println("string value = "+ss[i]);
		}
		System.out.println("sequence size = "+sd.length);
		for (int i = 0; i < sd.length; ++i) {
			System.out.print("sequence element["+i+"]: ");
			System.out.println("decimal value = "+sd[i]);
		}
		return 2.71828;
	}
	
	/** Returns a chuck containing an array of 'l' doubles */
	public static Object test3(int l)
	{
		return new ComplexChunk(l);
	}
	
	/** Takes a chunk from test3, mutates it and prints it */
	public static void test4(Object o)
	{
		ComplexChunk cc = (ComplexChunk) o;
		for (int i = 0; i < cc.size; ++i) {
			cc.data[i] = Math.sqrt(cc.data[i]);
		}
		cc.print();
	}
}

/**
  An object we will be passing out to store in a chunk in EPL, 
  to be passed back into the plugin later.
*/
class ComplexChunk
{
	long size;
	double[] data;
	
	public ComplexChunk(int size)
	{
		this.size = size;
		this.data = new double[size];
		for (int l = 0; l < size; ++l) {
			this.data[l] = l;
		}
		print();
	}
	public void print()
	{
		System.out.println("Chunk size = "+size);
		for (int l = 0; l < size; ++l) {
			System.out.println("Chunk element ["+l+"] = "+data[l]);
		}
	}
}
