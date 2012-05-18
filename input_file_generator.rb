def f(x)
  x * x * Math.sin(x)
end

File.open('input_file.txt', 'w') do |f|
  second_func_str = "f((x - 0.89) ** 1.12 - 2 * rand)"
  f << "x\tf(x)\n"
  f << "#\tf(x)\t#{second_func_str}\n"
  3.step(13, 0.1) do |x|
    f << "%.1f\t%.3f\t%.3f\n" % [x * 5, f(x), eval(second_func_str)]
  end
end
