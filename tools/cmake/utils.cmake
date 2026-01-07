# Generate source code from module xml file
function(utils_text_to_code input_dir input_file output_dir output_file)

  # Check if ${input_file} is an xml file
  get_filename_component(file_extension "${input_file}" LAST_EXT)
  if("${file_extension}" STREQUAL ".xml")
    # Generate minimal xml file
    generate_minimal_xml("${input_dir}" "${input_file}")
    set(input_dir "${input_dir}/minimal")
  endif()

  # Create output directory
  file(MAKE_DIRECTORY "${output_dir}")

  find_program(xxd xxd)
  if("${xxd}" MATCHES ".+-NOTFOUND")
    message(FATAL_ERROR "error: xxd not found (${xxd}).")
    return()
  endif()

  # Create char array with Hexdump from xml
  execute_process(
    COMMAND "${xxd}" -i "${input_file}" "${output_dir}/${output_file}"
    RESULT_VARIABLE exitcode
    WORKING_DIRECTORY "${input_dir}")

  if(NOT "${exitcode}" EQUAL 0)
    message(FATAL_ERROR "error: xxd status ${exitcode}")
    return()
  endif()

  find_program(sed sed)
  if("${sed}" MATCHES ".+-NOTFOUND")
    message(FATAL_ERROR "error: sed not found (${sed}).")
    return()
  endif()

  # Prepend header guard
  execute_process(
    COMMAND "${sed}" -i "1i #pragma once" "${output_file}"
    RESULT_VARIABLE exitcode
    WORKING_DIRECTORY "${output_dir}")

  if(NOT "${exitcode}" EQUAL 0)
    message(FATAL_ERROR "error: sed status ${exitcode}")
    return()
  endif()

endfunction(utils_text_to_code)

# Generate source code with initial seed
function(utils_create_seed output_dir output_file)

  # Generate random number
  execute_process(
    COMMAND
      bash -c
      "echo -n $(LC_ALL=C tr </dev/urandom -dc 'A-Z9' | fold -w 1024 | head -n 1)"
    OUTPUT_VARIABLE content)

  # Create output directory and save content to temporary file
  file(MAKE_DIRECTORY "${output_dir}")
  file(WRITE "${output_dir}/tmp_initial_seed" "${content}")

  # Create sourcecode with random number
  utils_text_to_code("${output_dir}" "tmp_initial_seed" "${output_dir}"
                     "${output_file}")

  # Remove temporary tmp_initial_seed file
  execute_process(COMMAND rm "tmp_initial_seed"
                  WORKING_DIRECTORY "${output_dir}")

endfunction(utils_create_seed)

# Generate source code with initial seed
function(generate_minimal_xml input_dir model_xml)

  find_program(awk awk mawk gawk)
  if("${awk}" MATCHES ".+-NOTFOUND")
    message(FATAL_ERROR "error: awk (and mawk and gawk) not found (${awk}).")
    return()
  endif()

  execute_process(
    COMMAND "${awk}" "/UAVariable/{print $0} /<Value>/,/<\\/Value>/{print $0}"
            "${model_xml}"
    RESULT_VARIABLE exitcode
    OUTPUT_VARIABLE content
    WORKING_DIRECTORY "${input_dir}")

  if(NOT "${exitcode}" EQUAL 0)
    message(FATAL_ERROR "error: awk status ${exitcode}")
    return()
  endif()

  # Create output directory and save content to temporary file
  file(MAKE_DIRECTORY "${input_dir}/minimal")
  file(WRITE "${input_dir}/minimal/${model_xml}" "${content}")

  find_program(sed sed)
  if("${sed}" MATCHES ".+-NOTFOUND")
    message(FATAL_ERROR "error: sed not found (${sed}).")
    return()
  endif()

  execute_process(
    COMMAND "${sed}" -ne "/<UAVariable/{N;/<Value>/{p}};/<UAVariable/!p"
            "${model_xml}"
    RESULT_VARIABLE exitcode
    OUTPUT_VARIABLE content
    WORKING_DIRECTORY "${input_dir}/minimal")
  if(NOT "${exitcode}" EQUAL 0)
    message(FATAL_ERROR "error: sed status ${exitcode}")
    return()
  endif()

  # Create minimal file
  set(xml "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n ${content}")
  file(WRITE "${input_dir}/minimal/${model_xml}" "${xml}")

endfunction(generate_minimal_xml)
