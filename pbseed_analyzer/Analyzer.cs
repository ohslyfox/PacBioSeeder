using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace pbseedAnalyzer
{
    public class Analyzer
    {
        public static void Main(string[] args)
        {
            List<string> FilesToAnalyze = new List<string>
            {
                "testsim/testsim-maxgroup-9-1.txt",
                "testsim/testsim-minscore-9-1.txt",
                "testsim/testsim-minframe-9-1.txt",
                "chr22/chr22-maxgroup-9-1.txt",
                "chr22/chr22-minscore-9-1.txt",
                "chr22/chr22-minframe-9-1.txt"
            };

            foreach (var file in FilesToAnalyze)
            {
                try
                {
                    AnalyzeFile(new ResultsFile(file));
                }
                catch
                {
                    Console.WriteLine($"Failed to analyze {file}.");
                }
            }
        }

        private static void AnalyzeFile(ResultsFile file)
        {
            StringBuilder sb = new StringBuilder();
            sb.AppendLine("--------------------");
            sb.AppendLine($"Analyzing file: {file.FileName}");
            sb.AppendLine("--------------------");
            int count = 0;
            foreach (var readNumber in file.Results)
            {
                count++;
                double average = 0d;
                int minDelta = Int32.MaxValue;
                int maxDelta = Int32.MinValue;
                foreach (var read in readNumber)
                {
                    average += read.Delta;
                    if (read.Delta < minDelta)
                    {
                        minDelta = read.Delta;
                    }
                    if (read.Delta > maxDelta)
                    {
                        maxDelta = read.Delta;
                    }
                }
                sb.AppendLine($"Read {count}:");
                sb.AppendLine($"Average delta: {average / readNumber.Count}");
                sb.AppendLine($"Max delta: {maxDelta}");
                sb.AppendLine($"Min delta: {minDelta}\n");
            }
            sb.AppendLine("\n");

            try
            {
                string fileName = file.FileName.Substring(0, file.FileName.Length - 4);
                fileName += "-analyer-results.txt";
                using (StreamWriter sw = File.CreateText(fileName))
                {
                    sw.Write(sb.ToString());
                }
            }
            catch { Environment.Exit(1); }
        }
    }

    public class ResultsFile
    {
        public string FileName { get; set; }
        public List<List<PacBioRead>> Results { get; set; }

        public ResultsFile(string fileToRead)
        {
            this.FileName = fileToRead;
            this.Results = new List<List<PacBioRead>>();
            ParseFile(fileToRead);
        }

        private void ParseFile(string fileToRead)
        {
            try
            {
                using (StreamReader sr = File.OpenText(fileToRead))
                {
                    int skipCount = 0;
                    int lineCount = 0;
                    string s;

                    List<string> linesToParse = new List<string>();
                    while ((s = sr.ReadLine()) != null)
                    {
                        if (skipCount < 9)
                        {
                            skipCount++;
                            continue;
                        }


                        if (lineCount % 4 != 3)
                        {
                            linesToParse.Add(s);
                        }
                        else
                        {
                            PacBioRead newRead = new PacBioRead(linesToParse);
                            AddReadToResults(newRead);
                            linesToParse.Clear();
                        }
                        lineCount++;
                    }
                }
            }
            catch
            {
                Console.WriteLine("An error occured while trying to read " + fileToRead);
                Environment.Exit(1);
            }
        }

        private void AddReadToResults(PacBioRead newRead)
        {
            if (this.Results.Count == 0)
            {
                AddReadList(newRead);
            }
            else
            {
                var currentReadList = this.Results[this.Results.Count - 1];
                var firstItem = currentReadList[0];

                if (firstItem.ReadNumber == newRead.ReadNumber)
                {
                    currentReadList.Add(newRead);
                }
                else
                {
                    AddReadList(newRead);
                }
            }
        }

        private void AddReadList(PacBioRead newRead)
        {
            var newReadList = new List<PacBioRead>();
            newReadList.Add(newRead);
            this.Results.Add(newReadList);
        }

        public class PacBioRead
        {
            public int ReadNumber { get; set; }
            public int ResultLineNumber { get; set; }
            public int SolutionLineNumber { get; set; }
            public int Delta { get; set; }

            public PacBioRead(List<string> readData)
            {
                ParseLines(readData);
            }

            private void ParseLines(List<string> readData)
            {
                for (int i = 0; i < readData.Count; i++)
                {
                    switch (i)
                    {
                        case 0:
                            ParseFirstLine(readData[i]);
                            break;
                        case 1:
                            ParseSecondLine(readData[i]);
                            break;
                        case 2:
                            ParseThirdLine(readData[i]);
                            break;
                    }
                }
            }

            private void ParseThirdLine(string thirdLine)
            {
                string[] parts = thirdLine.Split(" ");

                var resultPart = parts[2];
                resultPart = resultPart.Substring(0, resultPart.Length - 1);
                this.Delta = Int32.Parse(resultPart);
            }

            private void ParseSecondLine(string secondLine)
            {
                string[] parts = secondLine.Split(" ");

                var resultPart = parts[2];
                resultPart = resultPart.Substring(0, resultPart.Length - 1);
                this.SolutionLineNumber = Int32.Parse(resultPart);
            }

            private void ParseFirstLine(string firstLine)
            {
                string[] parts = firstLine.Split(" ");
                var readNumber = parts[2].Substring(1, parts[2].Length - 2);
                this.ReadNumber = Int32.Parse(readNumber);

                var resultPart = parts[5];
                resultPart = resultPart.Substring(0, resultPart.Length - 1);
                this.ResultLineNumber = Int32.Parse(resultPart);
            }
        }
    }
}
